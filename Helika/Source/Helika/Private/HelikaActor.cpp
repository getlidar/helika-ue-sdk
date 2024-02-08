
#include "HelikaActor.h"


AHelikaActor::AHelikaActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHelikaActor::BeginPlay()
{
	Super::BeginPlay();
    _gamerID = playerId;
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
        return;
    }

    _helikaApiKey = apiKeyIn;
    _gameId = gameIdIN;
    _baseUrl = ConvertUrl(env);
    _sessionID = FGuid::NewGuid().ToString();

    _enabled = env != HelikaEnvironment::Localhost ? enabled : false;

    CreateSession();
}

void AHelikaActor::SendHTTPPost(FString url, FString data)
{
    FString uriBase = _baseUrl + url;
    FHttpModule& httpModule = FHttpModule::Get();
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
            bool connectedSuccessfully) mutable {

        if (connectedSuccessfully) {

            ProcessEventTrackResponse(pResponse->GetContentAsString());
        }
        else {
            switch (pRequest->GetStatus()) {
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

    for (auto& Event : helikaEvents.events)
    {
        if (Event.game_id.IsEmpty())
            Event.game_id = _gameId;
        Event.created_at = FDateTime::UtcNow().ToIso8601();
        Event.event.Add("sessionID", _sessionID);
        Event.event.Add("gamer_id", _gamerID);
    }
    FString JSONPayload;
    FJsonObjectConverter::UStructToJsonObjectString(helikaEvents, JSONPayload, 0, 0);
    UE_LOG(LogTemp, Display, TEXT("Sent Helika Event : %s"), *JSONPayload);
    SendHTTPPost("/game/game-event", JSONPayload);
}

void AHelikaActor::CreateSession()
{
    FHEvents Fevents;
    Fevents.event_type = "SESSION_CREATED";
    Fevents.game_id = "HELIKA_SDK";

    Fevents.event.Add("sdk_name", "Unreal");
    Fevents.event.Add("sdk_version", "0.1.0");
    Fevents.event.Add("sdk_class", "HelikaActor");
    Fevents.event.Add("sessionID", _sessionID);

    TArray<FHEvents> Feventsarray;
    Feventsarray.Add(Fevents);

    FHSession Fsession;
    Fsession.events = Feventsarray;

    SendEvent(Fsession); 
}


