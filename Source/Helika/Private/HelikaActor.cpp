
#include "HelikaActor.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMisc.h"

AHelikaActor::AHelikaActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AHelikaActor::BeginPlay()
{
    Super::BeginPlay();
    _playerId = playerId;
    Init(apiKey, gameId, helikaEnv, telemetry, printEventsToConsole);
}

void AHelikaActor::SetPlayerID(FString InPlayerID)
{
    playerId = InPlayerID;
}

FString AHelikaActor::ConvertUrl(HelikaEnvironment baseUrl)
{
    switch (baseUrl)
    {
    case HelikaEnvironment::Production:
        return "https://api.helika.io/v1";
    case HelikaEnvironment::Develop:
        return "https://api-stage.helika.io/v1";
    case HelikaEnvironment::Localhost:
        return "http://localhost:8181/v1";
    default:
        return "http://localhost:8181/v1";
    }
}

FString AHelikaActor::GetDeviceType()
{   switch (GetPlatformType())
    {
    case EPlatformType::PT_IOS :
    case EPlatformType::PT_ANDROID :
        {
            return "Mobile";
        }
    case EPlatformType::PT_WINDOWS :
    case EPlatformType::PT_MAC :
    case EPlatformType::PT_LINUX :
        {
            return "Desktop";
        }
    case EPlatformType::PT_CONSOLE :
        {
            return "Console";
        }
    case EPlatformType::PT_DEFAULT :
    case EPlatformType::PT_UNKNOWN :
        {
            return "Unknown";            
        }
    }
        return FString();
}

FString AHelikaActor::GetDeviceProcessorType()
{
    switch (GetPlatformType())
    {
    case EPlatformType::PT_IOS :
    case EPlatformType::PT_ANDROID :
        {
            return FPlatformMisc::GetCPUChipset();
        }
    case EPlatformType::PT_WINDOWS :
    case EPlatformType::PT_MAC :
    case EPlatformType::PT_LINUX :
    case EPlatformType::PT_CONSOLE :
        {
            return FPlatformMisc::GetCPUBrand();
        }
    case EPlatformType::PT_DEFAULT :
    case EPlatformType::PT_UNKNOWN :
        {
            return FString();            
        }
    }
    return FString();
}

void AHelikaActor::Init(FString apiKeyIn, FString gameIdIN, HelikaEnvironment env, TelemetryLevel telemetryLevel, bool isPrintEventsToConsole)
{
    if (_isInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("HelikaActor is already initialized"));
        return;
    }

    if (gameIdIN.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Missing Game ID"));
        return;
    }

    if (apiKeyIn.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid API Key"));
        return;
    }

    _helikaApiKey = apiKeyIn;
    _gameId = gameIdIN;
    _baseUrl = ConvertUrl(env);
    _sessionID = FGuid::NewGuid().ToString();
    _isInitialized = true;

    _telemetry = env != HelikaEnvironment::Localhost ? telemetryLevel : TelemetryLevel::None;
    _printEventsToConsole = isPrintEventsToConsole;
    if (_telemetry > TelemetryLevel::TelemetryOnly)
    {
        // install event setup -> kochava
    }
    CreateSession();
}

void AHelikaActor::SendHTTPPost(FString url, FString data)
{
    if (_printEventsToConsole)
    {
        UE_LOG(LogTemp, Display, TEXT("Sent Helika Event : %s"), *data);
        return;
    }
    if (_telemetry > TelemetryLevel::None)
    {
        FString uriBase = _baseUrl + url;
        FHttpModule &httpModule = FHttpModule::Get();
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = httpModule.CreateRequest();

        pRequest->SetVerb(TEXT("POST"));
        pRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        pRequest->SetHeader(TEXT("x-api-key"), _helikaApiKey);

        FString RequestContent = data;
        pRequest->SetContentAsString(RequestContent);
        pRequest->SetURL(uriBase);
        pRequest->OnProcessRequestComplete().BindLambda(
            [&](
                FHttpRequestPtr pRequest,
                FHttpResponsePtr pResponse,
                bool connectedSuccessfully) mutable
            {
                if (connectedSuccessfully)
                {

                    ProcessEventTrackResponse(pResponse->GetContentAsString());
                }
                else
                {
                    switch (pRequest->GetStatus())
                    {
                    case EHttpRequestStatus::Failed_ConnectionError:
                        UE_LOG(LogTemp, Error, TEXT("Connection failed."));
                    default:
                        UE_LOG(LogTemp, Error, TEXT("Request failed."));
                    }
                }
            });

        pRequest->ProcessRequest();
    }
}

void AHelikaActor::ProcessEventTrackResponse(FString data)
{
    UE_LOG(LogTemp, Display, TEXT("Helika Server Responce : %s"), *data);
}

void AHelikaActor::SendEvent(FHSession helikaEvents)
{
    helikaEvents.id = _sessionID;

    for (auto &Event : helikaEvents.events)
    {
        if (Event.game_id.IsEmpty())
            Event.game_id = _gameId;
        Event.created_at = FDateTime::UtcNow().ToIso8601();
        Event.event.Add("session_id", _sessionID);
        Event.event.Add("player_id", _playerId);
    }
    FString JSONPayload;
    FJsonObjectConverter::UStructToJsonObjectString(helikaEvents, JSONPayload, 0, 0);
    SendHTTPPost("/game/game-event", JSONPayload);
}

void AHelikaActor::CreateSession()
{
    FHEvent fEvent;
    fEvent.event_type = "session_created";

    // Todo: Turn these into static variables
    fEvent.event.Add("sdk_name", _sdk_name);
    fEvent.event.Add("sdk_version", _sdk_version);
    fEvent.event.Add("sdk_class", _sdk_class);
    fEvent.event.Add("sdk_platform", GetPlatformName());
    fEvent.event.Add("event_sub_type", "session_created");
    fEvent.event.Add("telemetry_level", UEnum::GetDisplayValueAsText(_telemetry).ToString());

    if (_telemetry > TelemetryLevel::TelemetryOnly)
    {
        FString OSVersionLabel, OSSubVersionLabel;
        FPlatformMisc::GetOSVersions(OSVersionLabel,OSSubVersionLabel);
        fEvent.event.Add("os", OSVersionLabel + OSSubVersionLabel);
        fEvent.event.Add("os_family", GetPlatformName());
        fEvent.event.Add("device_model", FPlatformMisc::GetDeviceMakeAndModel());
        fEvent.event.Add("device_name", FPlatformProcess::ComputerName());
        fEvent.event.Add("device_type", GetDeviceType());
        fEvent.event.Add("device_ue_unique_identifier", GetDeviceUniqueIdentifier());
        fEvent.event.Add("device_processor_type", GetDeviceProcessorType());
    }

    TArray<FHEvent> fEventsArray;
    fEventsArray.Add(fEvent);

    FHSession Fsession;
    Fsession.events = fEventsArray;

    SendEvent(Fsession);
}

EPlatformType AHelikaActor::GetPlatformType()
{
#if PLATFORM_WINDOWS
    return EPlatformType::PT_WINDOWS;
#elif PLATFORM_IOS
    return EPlatformType::PT_IOS;
#elif PLATFORM_MAC
    return EPlatformType::PT_MAC;
#elif PLATFORM_ANDROID
    return EPlatformType::PT_ANDROID;
#elif PLATFORM_LINUX
    return EPlatformType::PT_LINUX;
#elif PLATFORM_CONSOLE
    return EPlatformType::PT_CONSOLE;
#else
    ensureMsgf(false, TEXT("Platform unknown"));
    return EPlatformType::PT_UNKNOWN;
#endif
}

FString AHelikaActor::GetPlatformName()
{
#if PLATFORM_WINDOWS
    return FString(TEXT("Windows"));
#elif PLATFORM_IOS
    return FString(TEXT("IOS"));
#elif PLATFORM_MAC
    return FString(TEXT("Mac"));
#elif PLATFORM_ANDROID
    return FString(TEXT("Android"));
#elif PLATFORM_LINUX
    return FString(TEXT("Linux"));
#elif PLATFORM_CONSOLE
    return FString(TEXT("Console"));
#else
    ensureMsgf(false, TEXT("Platform unknown"));
    return FString(TEXT("Unknown"));
#endif
}

///
/// In case of Windows and Mac we have access to operating system ID
/// In case of Android and iOS we have access to device ID
/// @return
FString AHelikaActor::GetDeviceUniqueIdentifier()
{
#if PLATFORM_WINDOWS || PLATFORM_MAC
    return FPlatformMisc::GetOperatingSystemId();
#elif PLATFORM_ANDROID || PLATFORM_IOS
    return FPlatformMisc::GetDeviceId();
#else
    return FPlatformMisc::GetDeviceId();
#endif
}


///
/// Sample Usage:
/// TSharedPtr<FJsonObject> EventData = MakeShareable(new FJsonObject());
/// EventData->SetStringField("String Field", "Helika");
/// EventData->SetNumberField("Number Field", 1234);
/// EventData->SetBoolField("Bool value", true);
/// SendCustomEvent(EventData);
/// 
/// @param eventProps Event data in form of json object
void AHelikaActor::SendCustomEvent(const TSharedPtr<FJsonObject>& eventProps)
{
    if(!_isInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("Helika Actor is not yet initialized"));
    }

    // adding unique id to event
    TSharedPtr<FJsonObject> newEvent = MakeShareable(new FJsonObject());
    newEvent->SetStringField("id", FGuid::NewGuid().ToString());
    newEvent->SetObjectField("events", eventProps);

    // converting Json object to string
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(newEvent.ToSharedRef(), Writer);

    // send event to helika API
    SendHTTPPost("/game/game-event", JsonString);
}
