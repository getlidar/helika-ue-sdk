
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
    Init(apiKey, gameId, helikaEnv, sendingEvents);
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

void AHelikaActor::Init(FString apiKeyIn, FString gameIdIN, HelikaEnvironment env, bool enabled)
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

    _enabled = env != HelikaEnvironment::Localhost ? enabled : false;

    CreateSession();
}

void AHelikaActor::SendHTTPPost(FString url, FString data)
{
    if (!_enabled)
    {
        UE_LOG(LogTemp, Display, TEXT("Sent Helika Event : %s"), *data);
        return;
    }

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
    fEvent.event.Add("sdk_name", "Unreal");
    fEvent.event.Add("sdk_version", "0.1.0");
    fEvent.event.Add("sdk_class", "HelikaActor");
    fEvent.event.Add("session_id", _sessionID);
    fEvent.event.Add("event_sub_type", "session_created");
    fEvent.event.Add("os", UGameplayStatics::GetPlatformName());
    fEvent.event.Add("device_model", FGenericPlatformMisc::GetDeviceMakeAndModel());
    fEvent.event.Add("device_ue_unique_identifier", FGenericPlatformMisc::GetDeviceId());

    // Todo: Add missing if applicable
    // fEvent.event.Add("sdk_platform", "");
    // fEvent.event.Add("os", "");
    // fEvent.event.Add("os_family", "");
    // fEvent.event.Add("device_model", "");
    // fEvent.event.Add("device_name", "");
    // fEvent.event.Add("device_type", "");
    // fEvent.event.Add("device_ue_unique_identifier", "");
    // fEvent.event.Add("device_processor_type", "");

    TArray<FHEvent> fEventsArray;
    fEventsArray.Add(fEvent);

    FHSession Fsession;
    Fsession.events = fEventsArray;

    SendEvent(Fsession);
}
