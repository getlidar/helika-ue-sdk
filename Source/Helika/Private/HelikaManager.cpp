// Fill out your copyright notice in the Description page of Project Settings.


#include "HelikaManager.h"

#include "HelikaDefines.h"
#include "HelikaJsonLibrary.h"
#include "HelikaLibrary.h"
#include "HelikaSettings.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#if PLATFORM_ANDROID

#include "Android/AndroidPlatformMisc.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Misc/OutputDeviceNull.h"

#endif
#if WITH_EDITOR
#include "Editor.h"
#endif


#if PLATFORM_IOS

#include <UIKit/UIKit.h>
#include <Foundation/Foundation.h>

#import <AppTrackingTransparency/AppTrackingTransparency.h>
#import <AdSupport/AdSupport.h>

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
	KochavaDeviceID = GenerateKochavaDeviceID();

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

void UHelikaManager::SendEvent(FString EventName, const FHelikaJsonObject& EventProps)
{
	SendEvent(EventName, EventProps.Object);
}

void UHelikaManager::SendEvents(FString EventName, TArray<FHelikaJsonObject> EventProps) const
{
	TArray<TSharedPtr<FJsonObject>> JsonArray;
	for (auto EventProp : EventProps)
	{
		JsonArray.Add(EventProp.Object);
	}

	SendEvents(EventName, JsonArray);
}

void UHelikaManager::SendCustomEvent(const FHelikaJsonObject& EventProps)
{
	SendCustomEvent(EventProps.Object);
}

void UHelikaManager::SendCustomEvents(TArray<FHelikaJsonObject> EventProps) const
{
	TArray<TSharedPtr<FJsonObject>> JsonArray;
	for (auto EventProp : EventProps)
	{
		JsonArray.Add(EventProp.Object);
	}

	SendCustomEvents(JsonArray);
	
} 

bool UHelikaManager::SendEvent(FString EventName, TSharedPtr<FJsonObject> EventProps) const
{
	if(!bIsInitialized)
	{
		UE_LOG(LogHelika, Error, TEXT("Helika Subsystem is not yet initialized"));
		return false;
	} 
	if(EventName.IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Name' cannot be empty"));
		return false;
	}
	
	FString TrimmedEvent = EventName.TrimStartAndEnd();
	if(TrimmedEvent.IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Name' trimmed cannot be empty"));
		return false;
	}

	if(!EventProps.IsValid())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Props' cannot be null"));
		return false;
	}
	
	
	// adding unique id to event
	const TSharedPtr<FJsonObject> FinalEvent = MakeShareable(new FJsonObject());
	FinalEvent->SetStringField("id", FGuid::NewGuid().ToString());
	
	TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
	const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(TrimmedEvent, EventProps)));
	EventArrayJsonObject.Add(JsonValueObject);
	FinalEvent->SetArrayField("events", EventArrayJsonObject);
	
	// converting Json object to string
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if(!FJsonSerializer::Serialize(FinalEvent.ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return false;
	}

	// send event to helika API
	SendHTTPPost("/game/game-event", JsonString);
	return true;
}

bool UHelikaManager::SendEvents(FString EventName, TArray<TSharedPtr<FJsonObject>> EventProps) const
{
	if(!bIsInitialized)
	{
		UE_LOG(LogHelika, Warning, TEXT("Helika Subsystem is not yet initialized"));
		return false;
	}

	if(EventProps.IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Props' cannot be empty"));
		return false;
	}

	if(EventName.IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Name' cannot be empty"));
		return false;
	}
	
	FString TrimmedEvent = EventName.TrimStartAndEnd();
	if(TrimmedEvent.IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Name' cannot be empty"));
		return false;
	}

	const TSharedPtr<FJsonObject> FinalEvent = MakeShareable(new FJsonObject());
	FinalEvent->SetStringField("id", FGuid::NewGuid().ToString());
    
	TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
	for (auto EventProp : EventProps)
	{
		if(!EventProp.IsValid())
		{
			UE_LOG(LogHelika, Error, TEXT("'Event Props' contains invalid/null object"));
			return false;
		}
		const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(TrimmedEvent, EventProp)));
		EventArrayJsonObject.Add(JsonValueObject);
	}        
	FinalEvent->SetArrayField("events", EventArrayJsonObject);
    
	// converting Json object to string
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if(!FJsonSerializer::Serialize(FinalEvent.ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return false;
	}

	// send event to helika API
	SendHTTPPost("/game/game-event", JsonString);

	return true;
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
bool UHelikaManager::SendCustomEvent(TSharedPtr<FJsonObject> EventProps) const
{
    if(!bIsInitialized)
    {
        UE_LOG(LogHelika, Log, TEXT("Helika Subsystem is not yet initialized"));
    	return false;
    }

	if(!EventProps.IsValid())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Props' cannot be null"));
		return false;
	}

	if(!EventProps->HasField(TEXT("event_type")) || EventProps->GetStringField(TEXT("event_type")).IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("Invalid Event: Missing 'event_type' field or empty 'event_type' field"));
		return false;
	}

    // adding unique id to event
    const TSharedPtr<FJsonObject> NewEvent = MakeShareable(new FJsonObject());
    NewEvent->SetStringField("id", FGuid::NewGuid().ToString());   

    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(EventProps)));
    EventArrayJsonObject.Add(JsonValueObject);
    NewEvent->SetArrayField("events", EventArrayJsonObject);

    // converting Json object to string
    FString JsonString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if(!FJsonSerializer::Serialize(NewEvent.ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return false;
	}
	
    // send event to helika API
    SendHTTPPost("/game/game-event", JsonString);
	return true;
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
bool UHelikaManager::SendCustomEvents(TArray<TSharedPtr<FJsonObject>> EventProps) const
{
    if(!bIsInitialized)
    {
        UE_LOG(LogHelika, Log, TEXT("Helika Subsystem is not yet initialized"));
    	return false;
    }
	
	if(EventProps.IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("'Event Props' cannot be empty"));
		return false;
	}

    const TSharedPtr<FJsonObject> NewEvent = MakeShareable(new FJsonObject());
    NewEvent->SetStringField("id", FGuid::NewGuid().ToString());
    
    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    for (auto EventProp : EventProps)
    {
    	if(!EventProp->HasField(TEXT("event_type")) || EventProp->GetStringField(TEXT("event_type")).IsEmpty())
    	{
    		UE_LOG(LogHelika, Error, TEXT("Invalid Event: Missing 'event_type' field or empty 'event_type' field"));
    		return false;
    	}
        const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(EventProp)));
        EventArrayJsonObject.Add(JsonValueObject);
    }        
    NewEvent->SetArrayField("events", EventArrayJsonObject);
    
    // converting Json object to string
    FString JsonString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if(!FJsonSerializer::Serialize(NewEvent.ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return false;
	}

    // send event to helika API
    SendHTTPPost("/game/game-event", JsonString); 

	return true;
}

void UHelikaManager::SetPlayerId(const FString& InPlayerId)
{
	if(InPlayerId.IsEmpty() || InPlayerId.TrimStartAndEnd().IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("Player Id cannot be set to empty"));
		return;
	}
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

bool UHelikaManager::IsSDKInitialized()
{
	return bIsInitialized;
}

FString UHelikaManager::GetSessionId() const
{
	return SessionId;
}

FString UHelikaManager::GetKochavaDeviceId()
{
	return KochavaDeviceID;
}

TSharedPtr<FJsonObject> UHelikaManager::AppendAttributesToJsonObject(TSharedPtr<FJsonObject> JsonObject) const
{
	// Add game_id only if the event doesn't already have it
	UHelikaLibrary::AddIfNull(JsonObject, "game_id", UHelikaLibrary::GetHelikaSettings()->GameId);
    
	// Convert to ISO 8601 format string using "o" specifier
	UHelikaLibrary::AddOrReplace(JsonObject, "created_at", FDateTime::UtcNow().ToIso8601());

	if(!JsonObject->HasField(TEXT("event")))
	{
		JsonObject->SetObjectField(TEXT("event"), MakeShareable(new FJsonObject()));
	}

	if(JsonObject->GetObjectField(TEXT("event")) == nullptr)
	{
		UE_LOG(LogHelika, Error, TEXT("Invalid Event: 'event' field must be of type [JsonObject]"));
	}

	const TSharedPtr<FJsonObject> InternalEvent = JsonObject->GetObjectField(TEXT("event"));
	UHelikaLibrary::AddOrReplace(InternalEvent, "session_id", SessionId);

	if(!UHelikaLibrary::GetHelikaSettings()->PlayerId.IsEmpty())
	{
		UHelikaLibrary::AddOrReplace(InternalEvent, "player_id", UHelikaLibrary::GetHelikaSettings()->PlayerId);
	}

	return JsonObject;
}

TSharedPtr<FJsonObject> UHelikaManager::AppendAttributesToJsonObject(const FString& EventName, const TSharedPtr<FJsonObject>& JsonObject) const
{
	TSharedPtr<FJsonObject> HelikaEvent = MakeShareable(new FJsonObject());
	HelikaEvent->SetStringField("game_id", UHelikaLibrary::GetHelikaSettings()->GameId);
	HelikaEvent->SetStringField("created_at", FDateTime::UtcNow().ToIso8601());
	HelikaEvent->SetStringField("event_type", EventName);

	if(!SessionId.IsEmpty())
	{
		JsonObject->SetStringField("session_id", SessionId);
	}
	if(!UHelikaLibrary::GetHelikaSettings()->PlayerId.IsEmpty())
	{
		JsonObject->SetStringField("player_id", UHelikaLibrary::GetHelikaSettings()->PlayerId);
	}

	HelikaEvent->SetObjectField("event", JsonObject);

	return HelikaEvent;
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
        InternalEvent->SetStringField("os", UHelikaLibrary::GetOSVersion());
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

    SendHTTPPost("/game/game-event", JSONPayload);
}

void UHelikaManager::SendHTTPPost(const FString& Url, const FString& Data) const
{
	if (UHelikaLibrary::GetHelikaSettings()->bPrintEventsToConsole)
	{
		UE_LOG(LogHelika, Display, TEXT("Sent Helika Event : %s"), *Data);
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
					UE_LOG(LogHelika, Error, TEXT("Request failed..! due to %s"), LexToString(Request->GetFailureReason()));
				}
			});

		PRequest->ProcessRequest();
	}
}

void UHelikaManager::ProcessEventTrackResponse(const FString& Data)
{
	UE_LOG(LogHelika, Display, TEXT("Helika Server Responce : %s"), *Data);
}

void UHelikaManager::EndSession(bool bIsSimulating)
{
	Get()->DeinitializeSDK();
}

FString UHelikaManager::GenerateKochavaDeviceID()
{
	FString KochavaDeviceId = FString::Printf(TEXT("KD%lldT%s"), UHelikaLibrary::GetUnixTimeLong(), *FGuid::NewGuid().ToString(EGuidFormats::Digits).ToUpper());
	
	return KochavaDeviceId;
}

void UHelikaManager::InitializeTracking()
{
#if PLATFORM_IOS

	if (@available(iOS 14, *)) {
		[ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
			switch (status) {
				case ATTrackingManagerAuthorizationStatusAuthorized: {
					UE_LOG(LogHelika, Log, TEXT("Tracking Authorized"));
					break;
				}
				case ATTrackingManagerAuthorizationStatusDenied: {
					UE_LOG(LogHelika, Log, TEXT("Tracking Denied"));
					break;
				}
				case ATTrackingManagerAuthorizationStatusNotDetermined: {
					UE_LOG(LogHelika, Log, TEXT("Tracking Not Determined"));
					break;
				}
				case ATTrackingManagerAuthorizationStatusRestricted: {
					UE_LOG(LogHelika, Log, TEXT("Tracking Restricted"));
					break;
				}
				default: {
					UE_LOG(LogHelika, Log, TEXT("Tracking Status unknown"));
					break;
				}
			}
		}];
	} else {
		UE_LOG(LogHelika, Log, TEXT("Tracking Authorized"));
	}	
#else
	UE_LOG(LogHelika, Log, TEXT("Tracking Different platform..!"));
#endif
	
}