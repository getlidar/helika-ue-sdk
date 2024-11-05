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

#include <openssl/sha.h>

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

	SessionExpiry = FDateTime::Now();
	PiiTracking = false;
	Enabled = true;
	AppDetails = NewObject<UAppDetails>();
	AppDetails->Initialize(FString(), FString(), FString(), FString(), FString());
	AnonId = GenerateAnonId();
	UserDetails = NewObject<UUserDetails>();
	UserDetails->Initialize(FString(), FString(), FString());
	
	CreateSession();
	
#if WITH_EDITOR
	FEditorDelegates::EndPIE.AddStatic(&EndEditorSession);
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

					ProcessEventSentSuccess(Response->GetContentAsString());
				}
				else
				{
					ProcessEventSentError(FString::Printf(TEXT("Request failed..! due to %s, Failure message %s"), LexToString(Request->GetFailureReason()), *Response->GetContentAsString()));
				}
			});

		PRequest->ProcessRequest();
	}
}

void UHelikaManager::ProcessEventSentSuccess(const FString& Data)
{
	UE_LOG(LogHelika, Log, TEXT("Event Processed Successfully : %s"), *Data);
}

void UHelikaManager::ProcessEventSentError(const FString& Data)
{
	UE_LOG(LogHelika, Display, TEXT("Helika Server Response : %s"), *Data);
}

void UHelikaManager::EndEditorSession(bool bIsSimulating)
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

TSharedPtr<FJsonObject> UHelikaManager::AppendHelikaData() const
{
	TSharedPtr<FJsonObject> HelikaData = MakeShareable(new FJsonObject());

	HelikaData->SetStringField("anon_id", AnonId);
	HelikaData->SetStringField("taxonomy_ver", "v2");
	HelikaData->SetStringField("sdk_name", UHelikaLibrary::GetHelikaSettings()->SDKName);
	HelikaData->SetStringField("sdk_version", UHelikaLibrary::GetHelikaSettings()->SDKVersion);
	HelikaData->SetStringField("sdk_platform", UHelikaLibrary::GetPlatformName());
	HelikaData->SetStringField("event_source", UHelikaLibrary::GetPlatformName());
	HelikaData->SetBoolField("pii_tracking", PiiTracking);

	return HelikaData;
}

TSharedPtr<FJsonObject> UHelikaManager::AppendPiiData(TSharedPtr<FJsonObject> HelikaData)
{
	TSharedPtr<FJsonObject> PiiData = MakeShareable(new FJsonObject());

	FVector2D GameResolution = UHelikaLibrary::GetGameResolution();
	
	PiiData->SetStringField("resolution", FString::Printf(TEXT("%fx%f"), GameResolution.X, GameResolution.Y));

	//TODO: Pending.
	PiiData->SetStringField("touch_support", FPlatformMisc::SupportsTouchInput() ? TEXT("True") : TEXT("False"));

	PiiData->SetStringField("device_type", UHelikaLibrary::GetDeviceType());
	PiiData->SetStringField("os", UHelikaLibrary::GetOSVersion());

	//TODO: Pending.
	//PiiData->SetStringField("downlink", UHelikaLibrary::GetPlatformName());
	//PiiData->SetStringField("effective_type", UHelikaLibrary::GetPlatformName());
	//PiiData->SetStringField("connection_type", UHelikaLibrary::GetPlatformName());

	HelikaData->SetObjectField("additional_user_info", PiiData);
	return HelikaData;
}

FDateTime UHelikaManager::AddHours(const FDateTime Date, const int Hours)
{
	FDateTime NewDateTime = Date + FTimespan(Hours, 0, 0);
	return NewDateTime;
}

FDateTime UHelikaManager::AddMinutes(const FDateTime Date, const int Minutes)
{
	FDateTime NewDateTime = Date + FTimespan(0, Minutes, 0);
	return NewDateTime;
}

void UHelikaManager::ExtendSession()
{
	SessionExpiry = FDateTime::Now() + FTimespan(0, 15, 0);
}

void UHelikaManager::EndSession() const
{
	TSharedPtr<FJsonObject> EndEvent = GetTemplateEvent("session_end", "session_end");
	EndEvent->SetStringField("event_type", "");
	
	TSharedPtr<FJsonObject> Event = EndEvent->GetObjectField(TEXT("event"));
	Event->SetStringField("event_sub_type", "session_end");
	Event->SetStringField("sdk_class", UHelikaLibrary::GetHelikaSettings()->SDKClass);
	Event->SetObjectField("helika_data", AppendHelikaData());
	Event->SetObjectField("app_details", AppDetails->ToJson());

	TSharedPtr<FJsonObject> EventParams = MakeShareable(new FJsonObject());
	EventParams->SetStringField("id", FGuid::NewGuid().ToString());
	TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
	const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(EndEvent));
	EventArrayJsonObject.Add(JsonValueObject);			
	EventParams->SetArrayField("events", EventArrayJsonObject);

	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if(!FJsonSerializer::Serialize(EventParams.ToSharedRef(), Writer))
	{
		UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
		return;
	}

	// send event to helika API
	SendHTTPPost("/game/game-event", JsonString);
}

TSharedPtr<FJsonObject> UHelikaManager::CreateInstallEvent()
{
	TSharedPtr<FJsonObject> InstallEvent = MakeShareable(new FJsonObject());
	InstallEvent->SetStringField("action", TEXT("install"));
	InstallEvent->SetStringField("kochava_app_id", UHelikaLibrary::GetHelikaSettings()->KochavaAppId);
	InstallEvent->SetStringField("kochava_device_id", GetKochavaDeviceId());

	TSharedPtr<FJsonObject> DataEvent = MakeShareable(new FJsonObject());
	DataEvent->SetStringField("origination_ip", UHelikaLibrary::GetLocalIpAddress());
	DataEvent->SetStringField("device_ua", FString::Printf(TEXT("Mozilla/5.0 ( %s )"), *UHelikaLibrary::GetOSVersion()));
	DataEvent->SetStringField("app_version", UHelikaLibrary::GetHelikaSettings()->AppVersion);

	TSharedPtr<FJsonObject> GdprEvent = MakeShareable(new FJsonObject());
	GdprEvent->SetNumberField("gdpr_applies", 1);
	GdprEvent->SetStringField("tc_string", "Hello");
	GdprEvent->SetNumberField("ad_user_data", 1);
	GdprEvent->SetNumberField("ad_personalization", 1);
	
	TSharedPtr<FJsonObject> DeviceIdEvent = MakeShareable(new FJsonObject());
	DeviceIdEvent->SetStringField("idfa", UHelikaLibrary::GetIdfa());
	DeviceIdEvent->SetStringField("idfv", UHelikaLibrary::GetIdfv());
	DeviceIdEvent->SetStringField("adid", UHelikaLibrary::GetAndroidAdID());
	DeviceIdEvent->SetStringField("android_id", UHelikaLibrary::GetDeviceUniqueIdentifier());

	DataEvent->SetObjectField("gdpr_privacy_consent", GdprEvent);
	DataEvent->SetObjectField("device_ids", DeviceIdEvent);

	InstallEvent->SetObjectField("data", DataEvent);

	return InstallEvent;
}

UUserDetails* UHelikaManager::GetUserDetails() const
{
	checkf(UserDetails, TEXT("User details are null"));
	return UserDetails;
}

void UHelikaManager::SetUserDetails(const FString& InUserId, const FString& InEmail, const FString& InWalletId, bool CreateNewAnon)
{
	UUserDetails* TempUserDetails = NewObject<UUserDetails>();
	if(InUserId.IsEmpty())
	{
		TempUserDetails->Initialize(GenerateAnonId(CreateNewAnon), "", "");
	}
	else
	{
		TempUserDetails->Initialize(InUserId, "", "");		
	}

	if(InEmail.IsEmpty() || !UHelikaLibrary::IsValidEmail(InEmail))
	{
		UE_LOG(LogHelika, Error, TEXT("Email is not a valid Email Address"));
		return;
	}
	TempUserDetails->Email = InEmail;

	if(InWalletId.IsEmpty() || !UHelikaLibrary::ValidateString(UHelikaLibrary::Wallet_Regex, InWalletId))
	{
		UE_LOG(LogHelika, Error, TEXT("Wallet address is not a valid address"));
		return;
	}
	TempUserDetails->WalletId = InWalletId;

	UserDetails = TempUserDetails;
}

UAppDetails* UHelikaManager::GetAppDetails() const
{
	checkf(AppDetails, TEXT("App details are null"));
	return AppDetails;
}

void UHelikaManager::SetAppDetails(const FString& InPlatformId, const FString& InCAV, const FString& InSAV, const FString& InStoreId,
                                   const FString& InSourceId)
{
	if(AppDetails == nullptr)
	{
		AppDetails = NewObject<UAppDetails>();
		AppDetails->Initialize(InPlatformId, InCAV, InSAV, InStoreId, InSourceId);
	}
	else
	{
		AppDetails->Initialize(InPlatformId, InCAV, InSAV, InStoreId, InSourceId);
	}
}

bool UHelikaManager::GetPiiTracking() const
{
	return PiiTracking;
}

void UHelikaManager::SetPiiTracking(bool InPiiTracking)
{
	PiiTracking = InPiiTracking;

	if(PiiTracking)
	{
		TSharedPtr<FJsonObject> PiiEvent = GetTemplateEvent("session_created", "session_data_updated");
		if(PiiEvent.IsValid())
		{
			TSharedPtr<FJsonObject> Event = PiiEvent->GetObjectField(TEXT("event"));

			Event->SetStringField("type", "Session Data Refresh");
			Event->SetStringField("sdk_class", UHelikaLibrary::GetHelikaSettings()->SDKClass);

			Event->SetObjectField("helika_data", AppendPiiData(AppendHelikaData()));
			
			Event->SetObjectField("app_details", AppDetails->ToJson());

			TSharedPtr<FJsonObject> EventParams = MakeShareable(new FJsonObject());
			EventParams->SetStringField("id", FGuid::NewGuid().ToString());
			TArray<TSharedPtr<FJsonValue>> EventArrayJsonObject;
			const TSharedPtr<FJsonValueObject> JsonValueObject = MakeShareable(new FJsonValueObject(PiiEvent));
			EventArrayJsonObject.Add(JsonValueObject);			
			EventParams->SetArrayField("events", EventArrayJsonObject);

			FString JsonString;
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			if(!FJsonSerializer::Serialize(EventParams.ToSharedRef(), Writer))
			{
				UE_LOG(LogHelika, Error, TEXT("Failed to serialize event data to JSON"));
				return;
			}

			// send event to helika API
			SendHTTPPost("/game/game-event", JsonString);
		}
	}
}

bool UHelikaManager::IsEnabled() const
{
	return Enabled;
}

void UHelikaManager::SetEnabled(const bool InEnabled)
{
	Enabled = InEnabled;
}

TSharedPtr<FJsonObject> UHelikaManager::GetTemplateEvent(FString EventType, FString EventSubType) const
{
	TSharedPtr<FJsonObject> TemplateEvent = MakeShareable(new FJsonObject());
	TemplateEvent->SetStringField("created_at", FDateTime::UtcNow().ToIso8601());
	TemplateEvent->SetStringField("game_id", UHelikaLibrary::GetHelikaSettings()->GameId);
	TemplateEvent->SetStringField("event_type", EventType);

	TSharedPtr<FJsonObject> TemplateSubEvent = MakeShareable(new FJsonObject());
	TemplateSubEvent->SetStringField("user_id", this->UserDetails->UserId);
	TemplateSubEvent->SetStringField("session_id", GetSessionId());
	if(!EventSubType.IsEmpty())
	{
		TemplateSubEvent->SetStringField("event_sub_type", EventSubType);
	}
	else
	{
		TemplateSubEvent->SetObjectField("event_sub_type", nullptr);
	}

	TemplateEvent->SetObjectField("event", TemplateSubEvent);

	return TemplateEvent;
}

FHelikaJsonObject UHelikaManager::GetTemplateEventAsHelikaJson(FString EventType, FString EventSubType)
{
	FHelikaJsonObject HelikaJsonObject;
	HelikaJsonObject.Object = GetTemplateEvent(EventType, EventSubType);
	return HelikaJsonObject;
}

FString UHelikaManager::GenerateAnonId(bool bBypassStored)
{
	FSHA256Signature Hash;

	FString DataS = FGuid::NewGuid().ToString();

	FTCHARToUTF8 Convertor(*DataS);
	const uint8* Data = reinterpret_cast<const uint8*>(Convertor.Get());
	int32 DataSize = Convertor.Length();
	
	SHA256_CTX SHA256_Context;
	SHA256_Init(&SHA256_Context);
	SHA256_Update(&SHA256_Context, Data, DataSize);
	SHA256_Final(Hash.Signature, &SHA256_Context);

	if(!bBypassStored)
	{
		if(AnonId.IsEmpty())
		{
			AnonId = "anon_" + Hash.ToString();
		}
		return AnonId;
	}
	
	return FString("anon_" + Hash.ToString());
}