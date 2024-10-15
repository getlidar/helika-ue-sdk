
#include "HelikaActor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "GenericPlatform/GenericPlatformMisc.h"

AHelikaActor::AHelikaActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AHelikaActor::BeginPlay()
{
    Super::BeginPlay();
    Init(ApiKey, GameId, HelikaEnvironment, Telemetry, bPrintEventsToConsole);
}

void AHelikaActor::Init(const FString& InApiKey, const FString& InGameId, const EHelikaEnvironment InHelikaEnvironment, const ETelemetryLevel InTelemetryLevel, const bool bInPrintEventsToConsole)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("HelikaActor is already initialized"));
        return;
    }

    if (InGameId.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Missing Game ID"));
        return;
    }

    if (InApiKey.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid API Key"));
        return;
    }

    BaseUrl = ConvertUrl(InHelikaEnvironment);
    SessionId = FGuid::NewGuid().ToString();
    bIsInitialized = true;

    // If Localhost is set, force print events
    Telemetry = InHelikaEnvironment != EHelikaEnvironment::HE_Localhost ? InTelemetryLevel : ETelemetryLevel::TL_None;

    // If PrintEventsToConsole is set to true, we only print the event to console, and we don't send it
    bPrintEventsToConsole = bInPrintEventsToConsole;
    
    if (Telemetry > ETelemetryLevel::TL_TelemetryOnly)
    {
        // install event setup -> kochava
    }
    CreateSession();
}

///
/// Sample Usage:
/// TSharedPtr<FJsonObject> EventData = MakeShareable(new FJsonObject());
/// EventData->SetStringField("String Field", "Helika");
/// EventData->SetNumberField("Number Field", 1234);
/// EventData->SetBoolField("Bool value", true);
/// SendCustomEvent(EventData);
/// 
/// @param EventProps Event data in form of json object
void AHelikaActor::SendCustomEvent(TSharedPtr<FJsonObject> EventProps) const
{
    if(!bIsInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("Helika Actor is not yet initialized"));
    }

    // adding unique id to event
    const TSharedPtr<FJsonObject> NewEvent = MakeShareable(new FJsonObject());
    NewEvent->SetStringField("id", FGuid::NewGuid().ToString());

    AppendAttributesToJsonObject(EventProps);

    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(EventProps));
    EventArrayJsonObject.Add(JsonValueObject);
    NewEvent->SetArrayField("events", EventArrayJsonObject);

    // converting Json object to string
    FString JsonString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(NewEvent.ToSharedRef(), Writer);

    // send event to helika API
    SendHTTPPost("/game/game-event", JsonString);
}

///
/// Sample Usage:
/// TArray<TSharedPtr<FJsonObject>> Array;
/// TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());
/// EventData1->SetStringField("event_type", "gameEvent");
/// EventData1->SetStringField("String Field", "Event1");
/// EventData1->SetNumberField("Number Field", 1234);
/// EventData1->SetBoolField("Bool value", true);
/// TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
/// EventData2->SetStringField("event_type", "gameEvent");
/// EventData2->SetStringField("String Field", "Event2");
/// EventData2->SetNumberField("Number Field", 1234);
/// EventData2->SetBoolField("Bool value", true);
/// Array.Add(EventData1);
/// Array.Add(EventData2);
/// SendCustomEvents(Array);
/// 
/// @param EventProps Event data in form of json[] object
void AHelikaActor::SendCustomEvents(TArray<TSharedPtr<FJsonObject>> EventProps) const
{
    if(!bIsInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("Helika Actor is not yet initialized"));
    }

    for (auto EventProp : EventProps)
    {
        AppendAttributesToJsonObject(EventProp);
    }

    const TSharedPtr<FJsonObject> NewEvent = MakeShareable(new FJsonObject());
    NewEvent->SetStringField("id", FGuid::NewGuid().ToString());
    
    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    for (auto EventProp : EventProps)
    {        
        const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(EventProp));
        EventArrayJsonObject.Add(JsonValueObject);
    }        
    NewEvent->SetArrayField("events", EventArrayJsonObject);
    
    // converting Json object to string
    FString JsonString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(NewEvent.ToSharedRef(), Writer);

    // send event to helika API
    SendHTTPPost("/game/game-event", JsonString);    
}

void AHelikaActor::SetPrintToConsole(const bool bInPrintEventsToConsole)
{
    bPrintEventsToConsole = bInPrintEventsToConsole;
}

FString AHelikaActor::GetPlayerId()
{
    return PlayerId;
}

void AHelikaActor::SetPlayerID(FString InPlayerID)
{
    PlayerId = InPlayerID;
}

void AHelikaActor::AppendAttributesToJsonObject(const TSharedPtr<FJsonObject>& JsonObject) const
{
    // Add game_id only if the event doesn't already have it
    AddIfNull(JsonObject, "game_id", GameId);
    
    // Convert to ISO 8601 format string using "o" specifier
    AddOrReplace(JsonObject, "created_at", FDateTime::UtcNow().ToIso8601());

    if(!JsonObject->HasField(TEXT("event_type")) || JsonObject->GetStringField(TEXT("event_type")).IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid Event: Missing 'event_type' field"));
    }

    if(!JsonObject->HasField(TEXT("event")))
    {
        JsonObject->SetObjectField(TEXT("event"), MakeShareable(new FJsonObject()));
    }

    if(JsonObject->GetObjectField(TEXT("event")) == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid Event: 'event' field must be of type [JsonObject]"));
    }

    const TSharedPtr<FJsonObject> InternalEvent = JsonObject->GetObjectField(TEXT("event"));
    AddOrReplace(InternalEvent, "session_id", SessionId);

    if(!PlayerId.IsEmpty())
    {
        AddOrReplace(InternalEvent, "player_id", PlayerId);
    }
}

void AHelikaActor::CreateSession() const
{
    const TSharedPtr<FJsonObject> InternalEvent = MakeShareable(new FJsonObject());
    InternalEvent->SetStringField("session_id", SessionId);
    InternalEvent->SetStringField("player_id", PlayerId);
    InternalEvent->SetStringField("sdk_name", SDKName);
    InternalEvent->SetStringField("sdk_version", SDKVersion);
    InternalEvent->SetStringField("sdk_class", SDKClass);
    InternalEvent->SetStringField("sdk_platform", GetPlatformName());
    InternalEvent->SetStringField("event_sub_type", "session_created");
    InternalEvent->SetStringField("telemetry_level", UEnum::GetValueAsString(Telemetry).RightChop(20));

    // TelemetryOnly means not sending Device, and Os information
    if (Telemetry > ETelemetryLevel::TL_TelemetryOnly)
    {
        FString OSVersionLabel, OSSubVersionLabel;
        FPlatformMisc::GetOSVersions(OSVersionLabel,OSSubVersionLabel);
        InternalEvent->SetStringField("os", OSVersionLabel + OSSubVersionLabel);
        InternalEvent->SetStringField("os_family", GetPlatformName());
        InternalEvent->SetStringField("device_model", FPlatformMisc::GetDeviceMakeAndModel());
        InternalEvent->SetStringField("device_name", FPlatformProcess::ComputerName());
        InternalEvent->SetStringField("device_type", GetDeviceType());
        InternalEvent->SetStringField("device_ue_unique_identifier", GetDeviceUniqueIdentifier());
        InternalEvent->SetStringField("device_processor_type", GetDeviceProcessorType());        
    }
    
    /// Creating json object for events
    const TSharedPtr<FJsonObject> CreateSessionEvent = MakeShareable(new FJsonObject());
    CreateSessionEvent->SetStringField("game_id", GameId);
    CreateSessionEvent->SetStringField("event_Type", "session_created");
    CreateSessionEvent->SetStringField("created_at", FDateTime::UtcNow().ToIso8601());
    CreateSessionEvent->SetObjectField("event", InternalEvent);
    
    const TSharedPtr<FJsonObject> Event = MakeShareable(new FJsonObject());
    Event->SetStringField("id", FGuid::NewGuid().ToString());
    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(CreateSessionEvent));
    EventArrayJsonObject.Add(JsonValueObject);
    Event->SetArrayField("events", EventArrayJsonObject);

    FString JSONPayload;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONPayload);
    FJsonSerializer::Serialize(Event.ToSharedRef(), Writer);
    
    SendHTTPPost("/game/game-event", JSONPayload);    
}

void AHelikaActor::SendHTTPPost(const FString& Url, const FString& Data) const
{
    if (bPrintEventsToConsole)
    {
        UE_LOG(LogTemp, Display, TEXT("Sent Helika Event : %s"), *Data);
        return;
    }
    if (Telemetry > ETelemetryLevel::TL_None)
    {
        const FString URIBase = BaseUrl + Url;
        FHttpModule &HTTPModule = FHttpModule::Get();
        const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PRequest = HTTPModule.CreateRequest();

        PRequest->SetVerb(TEXT("POST"));
        PRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
        PRequest->SetHeader(TEXT("x-api-key"), ApiKey);

        const FString RequestContent = Data;
        PRequest->SetContentAsString(RequestContent);
        PRequest->SetURL(URIBase);
        PRequest->OnProcessRequestComplete().BindLambda(
            [&](
            const FHttpRequestPtr& Request,
            const FHttpResponsePtr& Response,
            const bool bConnectedSuccessfully) mutable
            {
                if (bConnectedSuccessfully)
                {

                    ProcessEventTrackResponse(Response->GetContentAsString());
                }
                else
                {
                    switch (Request->GetFailureReason())
                    {
                    case EHttpFailureReason::ConnectionError:
                        UE_LOG(LogTemp, Error, TEXT("Connection failed."));
                    default:
                        UE_LOG(LogTemp, Error, TEXT("Request failed."));
                    }
                }
            });

        PRequest->ProcessRequest();
    }
}

void AHelikaActor::AddIfNull(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue)
{
    if(!HelikaEvent->HasField(Key))
    {
        HelikaEvent->SetStringField(Key, NewValue);
    }
}

void AHelikaActor::AddOrReplace(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue)
{
    if(HelikaEvent->HasField(Key))
    {
        HelikaEvent->SetStringField(Key, NewValue);
    }
    else
    {
        HelikaEvent->SetStringField(Key, NewValue);
    }
}

FString AHelikaActor::ConvertUrl(const EHelikaEnvironment InHelikaEnvironment)
{
    switch (InHelikaEnvironment)
    {
    case EHelikaEnvironment::HE_Production:
        return "https://api.helika.io/v1";
    case EHelikaEnvironment::HE_Develop:
        return "https://api-stage.helika.io/v1";
    case EHelikaEnvironment::HE_Localhost:
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

void AHelikaActor::ProcessEventTrackResponse(const FString& Data)
{
    UE_LOG(LogTemp, Display, TEXT("Helika Server Responce : %s"), *Data);
}