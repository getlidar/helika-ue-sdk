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

	if(UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey.IsEmpty() || UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey.TrimStartAndEnd().IsEmpty())
	{		
		UE_LOG(LogHelika, Error, TEXT("Helika API Key is empty. Please add the API key to editor settings"));
		return;
	}

	if(UHelikaLibrary::GetHelikaSettings()->GameId.IsEmpty() || UHelikaLibrary::GetHelikaSettings()->GameId.TrimStartAndEnd().IsEmpty())
	{		
		UE_LOG(LogHelika, Error, TEXT("Game ID is empty. Please add the API key to editor settings"));
		return;
	}

	BaseUrl = UHelikaLibrary::ConvertUrl(UHelikaLibrary::GetHelikaSettings()->HelikaEnvironment);
	SessionId = UHelikaLibrary::CreateNewGuid();
	bIsInitialized = true;

	AnonymousId = GenerateAnonymousId(SessionId, true);

	if(!UserDetails->HasField(TEXT("user_id")))
	{
		UserDetails->SetStringField("user_id", AnonymousId);
	}

	// If Localhost is set, force print events
	Telemetry = UHelikaLibrary::GetHelikaSettings()->HelikaEnvironment != EHelikaEnvironment::HE_Localhost ? UHelikaLibrary::GetHelikaSettings()->Telemetry : ETelemetryLevel::TL_None;

	if(Telemetry > ETelemetryLevel::TL_TelemetryOnly)
	{
		bPiiTracking = true;
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

void UHelikaManager::SendEvent(const FHelikaJsonObject& EventProps)
{
	SendEvent(EventProps.Object);
}

void UHelikaManager::SendEvents(TArray<FHelikaJsonObject> EventProps)
{
	TArray<TSharedPtr<FJsonObject>> JsonArray;
	for (auto EventProp : EventProps)
	{
		JsonArray.Add(EventProp.Object);
	}

	SendEvents(JsonArray);
}

void UHelikaManager::SendUserEvent(const FHelikaJsonObject& EventProps)
{
	SendUserEvent(EventProps.Object);
}

void UHelikaManager::SendUserEvents(TArray<FHelikaJsonObject> EventProps)
{
	TArray<TSharedPtr<FJsonObject>> JsonArray;
	for (auto EventProp : EventProps)
	{
		JsonArray.Add(EventProp.Object);
	}

	SendUserEvents(JsonArray);
	
} 

bool UHelikaManager::SendEvent(TSharedPtr<FJsonObject> EventProps)
{
	if(!bIsInitialized)
	{
		UE_LOG(LogHelika, Error, TEXT("Helika Subsystem is not yet initialized"));
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
	const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(EventProps, false)));
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

bool UHelikaManager::SendEvents(TArray<TSharedPtr<FJsonObject>> EventProps)
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
		const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(EventProp, false)));
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
/// SendUserEvent(EventData);
/// 
/// @param EventProps Event data in form of json object
bool UHelikaManager::SendUserEvent(TSharedPtr<FJsonObject> EventProps)
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

    // adding unique id to event
    const TSharedPtr<FJsonObject> FinalEvent = MakeShareable(new FJsonObject());
    FinalEvent->SetStringField("id", FGuid::NewGuid().ToString());   

    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(EventProps, true)));
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
/// SendUserEvents(Array);
/// 
/// @param EventProps Event data in form of json[] object
bool UHelikaManager::SendUserEvents(TArray<TSharedPtr<FJsonObject>> EventProps)
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

    const TSharedPtr<FJsonObject> FinalEvent = MakeShareable(new FJsonObject());
    FinalEvent->SetStringField("id", FGuid::NewGuid().ToString());
    
    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    for (auto EventProp : EventProps)
    {
        const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(AppendAttributesToJsonObject(EventProp, true)));
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

TSharedPtr<FJsonObject> UHelikaManager::AppendAttributesToJsonObject(TSharedPtr<FJsonObject> JsonObject, bool bIsUserEvent)
{
	// Add game_id only if the event doesn't already have it
	UHelikaLibrary::AddOrReplace(JsonObject, "game_id", UHelikaLibrary::GetHelikaSettings()->GameId);
    
	// Convert to ISO 8601 format string using "o" specifier
	UHelikaLibrary::AddOrReplace(JsonObject, "created_at", FDateTime::UtcNow().ToIso8601());

	if(!JsonObject->HasField(TEXT("event_type")) || JsonObject->GetStringField(TEXT("event_type")).IsEmpty() || JsonObject->GetStringField(TEXT("event_type")).TrimStartAndEnd().IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("Invalid Event: Missing 'event_type' field"));
	}

	if(!JsonObject->HasField(TEXT("event")))
	{
		JsonObject->SetObjectField(TEXT("event"), MakeShareable(new FJsonObject()));
	}

	if(JsonObject->GetObjectField(TEXT("event")) == nullptr)
	{
		UE_LOG(LogHelika, Error, TEXT("Invalid Event: 'event' field must be of type [JsonObject]"));
	}

	if(!JsonObject->GetObjectField(TEXT("event"))->HasField(TEXT("event_sub_type")) || JsonObject->GetObjectField(TEXT("event"))->GetStringField(TEXT("event_sub_type")).IsEmpty() || JsonObject->GetObjectField(TEXT("event"))->GetStringField(TEXT("event_sub_type")).TrimStartAndEnd().IsEmpty())
	{
		UE_LOG(LogHelika, Error, TEXT("Invalid Event: Missing 'event_sub_type' field"));
	}

	const TSharedPtr<FJsonObject> InternalEvent = JsonObject->GetObjectField(TEXT("event"));
	UHelikaLibrary::AddOrReplace(InternalEvent, "session_id", GetSessionId());

	UHelikaLibrary::AddOrReplace(InternalEvent, "user_id", bIsUserEvent ? UserDetails->GetStringField(TEXT("user_id")) : AnonymousId);

	AppendHelikaData(InternalEvent);
	AppendAppDetails(InternalEvent);

	if(bIsUserEvent)
	{
		AppendUserDetails(InternalEvent);
	}

	return JsonObject;
}

void UHelikaManager::CreateSession()
{

	TSharedPtr<FJsonObject> CreateSessionEvent = GetTemplateEvent("session_created", "session_created");
	TSharedPtr<FJsonObject> InternalEvent = CreateSessionEvent->GetObjectField(TEXT("event"));
	AppendHelikaData(InternalEvent);
	AppendUserDetails(InternalEvent);
	AppendAppDetails(InternalEvent);
	if(bPiiTracking)
	{
		AppendPIITracking(InternalEvent);
	}
    
    const TSharedPtr<FJsonObject> Event = MakeShareable(new FJsonObject());
    Event->SetStringField("id", FGuid::NewGuid().ToString());
    TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
    const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(CreateSessionEvent));
    EventArrayJsonObject.Add(JsonValueObject);
    Event->SetArrayField("events", EventArrayJsonObject);

    FString JSONPayload;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONPayload);
	if(!FJsonSerializer::Serialize(Event.ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return;
	}

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

FString UHelikaManager::GenerateAnonymousId(FString Seed, bool bCreateNewAnonId)
{
	if(bCreateNewAnonId)
	{
		return "anon_" + UHelikaLibrary::ComputeSha256Hash(Seed);
	}
	return AnonymousId;
}

TSharedPtr<FJsonObject> UHelikaManager::GetTemplateEvent(const FString& EventType, const FString& EventSubType) const
{
	TSharedPtr<FJsonObject> TemplateEvent = MakeShareable(new FJsonObject());
	TemplateEvent->SetStringField("created_at", FDateTime::UtcNow().ToIso8601());
	TemplateEvent->SetStringField("game_id", UHelikaLibrary::GetHelikaSettings()->GameId);
	TemplateEvent->SetStringField("event_type", EventType);
	
	TSharedPtr<FJsonObject> TemplateSubEvent = MakeShareable(new FJsonObject());
    TemplateSubEvent->SetStringField("user_id", UserDetails->GetStringField(TEXT("user_id")));
	TemplateSubEvent->SetStringField("session_id", GetSessionId());
	TemplateSubEvent->SetStringField("event_sub_type", EventSubType);
	TemplateSubEvent->SetObjectField("event_detail", MakeShareable(new FJsonObject()));

	TemplateEvent->SetObjectField("event", TemplateSubEvent);

	return TemplateEvent;
}

void UHelikaManager::AppendHelikaData(const TSharedPtr<FJsonObject>& GameEvent) const
{
	const TSharedPtr<FJsonObject> HelikaData = MakeShareable(new FJsonObject());

	HelikaData->SetStringField("anon_id", AnonymousId);
	HelikaData->SetStringField("taxonomy_ver", "v2");
	HelikaData->SetStringField("sdk_name", UHelikaLibrary::GetHelikaSettings()->SDKName);
	HelikaData->SetStringField("sdk_version", UHelikaLibrary::GetHelikaSettings()->SDKVersion);
	HelikaData->SetStringField("sdk_class", UHelikaLibrary::GetHelikaSettings()->SDKClass);
	HelikaData->SetStringField("sdk_platform", UHelikaLibrary::GetPlatformName());
	HelikaData->SetStringField("event_source", "client");
	HelikaData->SetBoolField("pii_tracking", bPiiTracking);

	UHelikaLibrary::AddIfNull(GameEvent, "helika_data", MakeShareable(new FJsonObject()));
	UHelikaJsonLibrary::MergeJObjects(GameEvent->GetObjectField(TEXT("helika_data")), HelikaData);
}

void UHelikaManager::AppendUserDetails(const TSharedPtr<FJsonObject>& GameEvent) const
{
	UHelikaLibrary::AddIfNull(GameEvent, "user_details", MakeShareable(new FJsonObject()));
	UHelikaJsonLibrary::MergeJObjects(GameEvent->GetObjectField(TEXT("user_details")), UserDetails);
}

void UHelikaManager::AppendAppDetails(const TSharedPtr<FJsonObject>& GameEvent) const
{
	UHelikaLibrary::AddIfNull(GameEvent, "app_details", MakeShareable(new FJsonObject()));
	UHelikaJsonLibrary::MergeJObjects(GameEvent->GetObjectField(TEXT("app_details")), AppDetails);
}

void UHelikaManager::AppendPIITracking(const TSharedPtr<FJsonObject>& GameEvent)
{
	const TSharedPtr<FJsonObject> PiiData = MakeShareable(new FJsonObject());

	PiiData->SetStringField("os", UHelikaLibrary::GetOSVersion());
	PiiData->SetStringField("os_family", UHelikaLibrary::GetPlatformName());
	PiiData->SetStringField("device_model", FPlatformMisc::GetDeviceMakeAndModel());
	PiiData->SetStringField("device_name", FPlatformProcess::ComputerName());
	PiiData->SetStringField("device_type", UHelikaLibrary::GetDeviceType());
	PiiData->SetStringField("device_ue_unique_identifier", UHelikaLibrary::GetDeviceUniqueIdentifier());
	PiiData->SetStringField("device_processor_type", UHelikaLibrary::GetDeviceProcessor());

	UHelikaLibrary::AddIfNull(GameEvent, "helika_data", MakeShareable(new FJsonObject()));
	UHelikaLibrary::AddOrReplace(GameEvent->GetObjectField(TEXT("helika_data")), "additional_user_info", PiiData);
}

TSharedPtr<FJsonObject> UHelikaManager::GetUserDetails()
{
	return UserDetails;
}

void UHelikaManager::SetUserDetails(TSharedPtr<FJsonObject> InUserDetails, bool bCreateNewAnonId)
{
	if(!InUserDetails->HasField(TEXT("user_id")) || InUserDetails->GetStringField(TEXT("user_id")).IsEmpty())
	{
		AnonymousId = GenerateAnonymousId(FGuid::NewGuid().ToString(), bCreateNewAnonId);
		InUserDetails = MakeShareable(new FJsonObject());
		InUserDetails->SetStringField("user_id", AnonymousId);
		InUserDetails->SetObjectField("email", nullptr);
		InUserDetails->SetObjectField("wallet", nullptr);
	}
	UserDetails = InUserDetails;
}

FHelikaJsonObject UHelikaManager::GetUserDetailsAsJson()
{
	FHelikaJsonObject UserDetailsJsonObject;
	UserDetailsJsonObject.Object = GetUserDetails();
	return UserDetailsJsonObject;
}

void UHelikaManager::SetUserDetails(const FHelikaJsonObject& InUserDetails, bool bCreateNewAnonId)
{
	SetUserDetails(InUserDetails.Object, bCreateNewAnonId);
}

TSharedPtr<FJsonObject> UHelikaManager::GetAppDetails()
{
	return AppDetails;
}

void UHelikaManager::SetAppDetails(const TSharedPtr<FJsonObject>& InAppDetails)
{
	AppDetails = InAppDetails;
}

FHelikaJsonObject UHelikaManager::GetAppDetailsAsJson()
{
	FHelikaJsonObject AppDetailsJsonObject;
	AppDetailsJsonObject.Object = GetAppDetails();
	return AppDetailsJsonObject;
}

void UHelikaManager::SetAppDetails(const FHelikaJsonObject& InAppDetails)
{
	SetAppDetails(InAppDetails.Object);
}

bool UHelikaManager::GetPiiTracking() const
{
	return bPiiTracking;
}

void UHelikaManager::SetPiiTracking(bool bInPiiTracking, bool bSendPiiTrackingEvent)
{
	bPiiTracking = bInPiiTracking;

	if(bIsInitialized && bPiiTracking && bSendPiiTrackingEvent)
	{
		TSharedPtr<FJsonObject> CreateSessionEvent = GetTemplateEvent("session_created", "session_data_updated");
		TSharedPtr<FJsonObject> InnerEvent = CreateSessionEvent->GetObjectField(TEXT("event"));
		UHelikaLibrary::AddIfNull(InnerEvent, "type", "Session Data Refresh");
		AppendHelikaData(InnerEvent);
		AppendUserDetails(InnerEvent);
		AppendAppDetails(InnerEvent);
		AppendPIITracking(InnerEvent);

		const TSharedPtr<FJsonObject> Event = MakeShareable(new FJsonObject());
		Event->SetStringField("id", FGuid::NewGuid().ToString());
		TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
		const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(CreateSessionEvent));
		EventArrayJsonObject.Add(JsonValueObject);
		Event->SetArrayField("events", EventArrayJsonObject);

		FString JSONPayload;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONPayload);
		if(!FJsonSerializer::Serialize(Event.ToSharedRef(), Writer))
		{
			UE_LOG(LogHelika, Error, TEXT("Unable to serialize Session event object..!"));
			return;
		}
		
		SendHTTPPost("/game/game-event", JSONPayload);
	}
}
