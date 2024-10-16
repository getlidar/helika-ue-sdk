// Fill out your copyright notice in the Description page of Project Settings.


#include "HelikaManager.h"

#include "HelikaDefines.h"
#include "HelikaLibrary.h"
#include "HelikaSettings.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#if WITH_EDITOR
#include "Editor.h"
#endif

UHelikaManager* UHelikaManager::Instance = nullptr;

void UHelikaManager::InitializeSDK()
{
	if(bIsInitialized)
	{		
		UE_LOG(LogHelika, Log, TEXT("HelikaActor is already initialized"));
		return;
	}

	if(UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey.IsEmpty())
	{		
		UE_LOG(LogHelika, Error, TEXT("Helika API Key is empty. Please add the API key to editor settings"));
		return;
	}

	if(UHelikaLibrary::GetHelikaSettings()->GameId.IsEmpty())
	{		
		UE_LOG(LogHelika, Error, TEXT("Game ID is empty. Please add the API key to editor settings"));
		return;
	}

	BaseUrl = UHelikaLibrary::ConvertUrl(UHelikaLibrary::GetHelikaSettings()->HelikaEnvironment);
	SessionId = UHelikaLibrary::CreateNewGuid();
	bIsInitialized = true;

	// If Localhost is set, force print events
	Telemetry = UHelikaLibrary::GetHelikaSettings()->HelikaEnvironment != EHelikaEnvironment::HE_Localhost ? UHelikaLibrary::GetHelikaSettings()->Telemetry : ETelemetryLevel::TL_None;

	if(Telemetry > ETelemetryLevel::TL_TelemetryOnly)
	{
		// install event setup -> kochava
	}
	
	CreateSession();
	
#if WITH_EDITOR
	FEditorDelegates::EndPIE.AddStatic(&EndSession);
#endif
}

void UHelikaManager::DeinitializeSDK()
{
	BaseUrl = "";
	SessionId = "";
	Telemetry = ETelemetryLevel::TL_None;
	bIsInitialized = false;	
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
void UHelikaManager::SendCustomEvent(TSharedPtr<FJsonObject> EventProps) const
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
void UHelikaManager::SendCustomEvents(TArray<TSharedPtr<FJsonObject>> EventProps) const
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

void UHelikaManager::SetPlayerId(const FString& InPlayerId)
{
	UHelikaLibrary::GetHelikaSettings()->PlayerId = InPlayerId;
}

FString UHelikaManager::GetPlayerId()
{
	return UHelikaLibrary::GetHelikaSettings()->PlayerId;
}

void UHelikaManager::SetPrintToConsole(bool bInPrintEventsToConsole)
{
	UHelikaLibrary::GetHelikaSettings()->bPrintEventsToConsole = bInPrintEventsToConsole;
}

void UHelikaManager::AppendAttributesToJsonObject(const TSharedPtr<FJsonObject>& JsonObject) const
{
	// Add game_id only if the event doesn't already have it
	UHelikaLibrary::AddIfNull(JsonObject, "game_id", UHelikaLibrary::GetHelikaSettings()->GameId);
    
	// Convert to ISO 8601 format string using "o" specifier
	UHelikaLibrary::AddOrReplace(JsonObject, "created_at", FDateTime::UtcNow().ToIso8601());

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
	UHelikaLibrary::AddOrReplace(InternalEvent, "session_id", SessionId);

	if(!UHelikaLibrary::GetHelikaSettings()->PlayerId.IsEmpty())
	{
		UHelikaLibrary::AddOrReplace(InternalEvent, "player_id", UHelikaLibrary::GetHelikaSettings()->PlayerId);
	}
}

void UHelikaManager::CreateSession() const
{	
    const TSharedPtr<FJsonObject> InternalEvent = MakeShareable(new FJsonObject());
    InternalEvent->SetStringField("session_id", SessionId);
    InternalEvent->SetStringField("player_id", UHelikaLibrary::GetHelikaSettings()->PlayerId);
    InternalEvent->SetStringField("sdk_name", UHelikaLibrary::GetHelikaSettings()->SDKName);
    InternalEvent->SetStringField("sdk_version", UHelikaLibrary::GetHelikaSettings()->SDKVersion);
    InternalEvent->SetStringField("sdk_class", UHelikaLibrary::GetHelikaSettings()->SDKClass);
    InternalEvent->SetStringField("sdk_platform", UHelikaLibrary::GetPlatformName());
    InternalEvent->SetStringField("event_sub_type", "session_created");
    InternalEvent->SetStringField("telemetry_level", UEnum::GetValueAsString(Telemetry).RightChop(20));

    // TelemetryOnly means not sending Device, and Os information
    if (Telemetry > ETelemetryLevel::TL_TelemetryOnly)
    {
        FString OSVersionLabel, OSSubVersionLabel;
        FPlatformMisc::GetOSVersions(OSVersionLabel,OSSubVersionLabel);
        InternalEvent->SetStringField("os", OSVersionLabel + OSSubVersionLabel);
        InternalEvent->SetStringField("os_family", UHelikaLibrary::GetPlatformName());
        InternalEvent->SetStringField("device_model", FPlatformMisc::GetDeviceMakeAndModel());
        InternalEvent->SetStringField("device_name", FPlatformProcess::ComputerName());
        InternalEvent->SetStringField("device_type", UHelikaLibrary::GetDeviceType());
        InternalEvent->SetStringField("device_ue_unique_identifier", UHelikaLibrary::GetDeviceUniqueIdentifier());
        InternalEvent->SetStringField("device_processor_type", UHelikaLibrary::GetDeviceProcessor());        
    }
    
    /// Creating json object for events
    const TSharedPtr<FJsonObject> CreateSessionEvent = MakeShareable(new FJsonObject());
    CreateSessionEvent->SetStringField("game_id", UHelikaLibrary::GetHelikaSettings()->GameId);
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

	UE_LOG(LogHelika, Log, TEXT("Helika Event : %s"), *JSONPayload);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Helika Event : %s"), *JSONPayload));
    SendHTTPPost("/game/game-event", JSONPayload);
}

void UHelikaManager::SendHTTPPost(const FString& Url, const FString& Data) const
{
	if (UHelikaLibrary::GetHelikaSettings()->bPrintEventsToConsole)
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
		PRequest->SetHeader(TEXT("x-api-key"), UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey);

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

void UHelikaManager::ProcessEventTrackResponse(const FString& Data)
{
	UE_LOG(LogTemp, Display, TEXT("Helika Server Responce : %s"), *Data);
}

void UHelikaManager::EndSession(bool bIsSimulating)
{
	Get()->DeinitializeSDK();
}
