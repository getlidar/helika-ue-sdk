// Copyright Epic Games, Inc. All Rights Reserved.

#include "HelikaDefines.h"
#include "HelikaLibrary.h"
#include "Misc/AutomationTest.h"
#include "HelikaManager.h"
#include "HelikaSettings.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelikaInitializeSDKTest, "Helika.HelikaInitializeSDKTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FHelikaInitializeSDKTest::RunTest(const FString& Parameters)
{
	ELogVerbosity::Type OriginalVerbosity = LogHelika.GetVerbosity();
	LogHelika.SetVerbosity(ELogVerbosity::NoLogging);
	
	UHelikaManager* HelikaManager = NewObject<UHelikaManager>();
	
	UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey = ""; // Missing API key
	HelikaManager->InitializeSDK();
	TestTrue("Initialization fails when API key is empty", !HelikaManager->IsSDKInitialized());
	
	UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey = "TestAPIKey"; // Ensure API key is present
	UHelikaLibrary::GetHelikaSettings()->GameId = ""; // Missing Game ID
	HelikaManager->InitializeSDK();
	TestTrue("Initialization fails when GameId is empty", !HelikaManager->IsSDKInitialized());
	
	UHelikaLibrary::GetHelikaSettings()->GameId = "ValidGameId";
	HelikaManager->InitializeSDK();
	TestTrue("Initialization succeeds when API key and GameId are valid", HelikaManager->IsSDKInitialized());
	
	TestTrue("Session Id should be created", !HelikaManager->GetSessionId().IsEmpty());
	
	LogHelika.SetVerbosity(OriginalVerbosity);	

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelikaSendEventTest, "Helika.HelikaSendEventTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FHelikaSendEventTest::RunTest(const FString& Parameters)
{
	ELogVerbosity::Type OriginalVerbosity = LogHelika.GetVerbosity();
	LogHelika.SetVerbosity(ELogVerbosity::NoLogging);

	
	UHelikaManager* HelikaManager = NewObject<UHelikaManager>();
	UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey = "TestAPIKey";
	UHelikaLibrary::GetHelikaSettings()->GameId = "ValidGameId";

	// calling before initialization
	TestTrue("Helika SDK is not initialized", !HelikaManager->SendEvent("HelikaEvent", TSharedPtr<FJsonObject>()));
	TestTrue("Helika SDK is not initialized", !HelikaManager->SendEvents("HelikaArrayEvent", TArray<TSharedPtr<FJsonObject>>()));
	
	HelikaManager->InitializeSDK();
	
	// InValid Parameters
	TestTrue("Event data cannot be null or empty" , !HelikaManager->SendEvent("HelikaEvent", nullptr));
	TestTrue("Passing empty event name" , !HelikaManager->SendEvent("", nullptr));
	TestTrue("Event name cannot only contains spaces", !HelikaManager->SendEvent("             ", nullptr));
	

	// Valid Parameters
	TestTrue("Event data cannot be null", HelikaManager->SendEvent("HelikaEvent", MakeShareable(new FJsonObject())));
	
	TSharedPtr<FJsonObject> EventData = MakeShareable(new FJsonObject());
	EventData->SetStringField("String Field", "Helika");
	EventData->SetNumberField("Number Field", 1234);
	EventData->SetBoolField("Bool value", true);
	EventData->SetObjectField("eventObject", nullptr);
	EventData->SetArrayField("eventArray", TArray<TSharedPtr<FJsonValue>>());

	TestTrue("Invalid Parameter Call", HelikaManager->SendEvent("HelikaEvent", EventData));




	//Testing SendEvents

	// InValid Parameters
	TestTrue("Event data cannot be null or empty" , !HelikaManager->SendEvents("HelikaArrayEvent", TArray<TSharedPtr<FJsonObject>>()));
	TestTrue("Passing empty event name" , !HelikaManager->SendEvents("", TArray<TSharedPtr<FJsonObject>>()));
	TestTrue("Event name cannot only contains spaces", !HelikaManager->SendEvents("             ", TArray<TSharedPtr<FJsonObject>>()));

	// Valid Parameters
	{
		TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());
		TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(EventData1);
		EventArray.Add(EventData2);
		
		TestTrue("Invalid Parameter Call", HelikaManager->SendEvents("HelikaArrayEvent", EventArray));		
	}

	{
		TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());
		EventData1->SetStringField("String Field", "Helika");
		EventData1->SetNumberField("Number Field", 1234);
		EventData1->SetBoolField("Bool value", true);
		TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(EventData1);
		EventArray.Add(EventData2);
		
		TestTrue("Invalid Parameter Call", HelikaManager->SendEvents("HelikaArrayEvent", EventArray));		
	}
	
	{
		TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());
		EventData1->SetStringField("String Field", "Helika");
		EventData1->SetNumberField("Number Field", 1234);
		EventData1->SetBoolField("Bool value", true);
		TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
		EventData2->SetStringField("String Field", "Helika");
		EventData2->SetNumberField("Number Field", 1234);
		EventData2->SetBoolField("Bool value", true);
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(EventData1);
		EventArray.Add(EventData2);
		
		TestTrue("Invalid Parameter Call", HelikaManager->SendEvents("HelikaArrayEvent", EventArray));		
	}
	
	

	
	LogHelika.SetVerbosity(OriginalVerbosity);	
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelikaSendCustomEventTest, "Helika.HelikaSendCustomEventTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FHelikaSendCustomEventTest::RunTest(const FString& Parameters)
{
	ELogVerbosity::Type OriginalVerbosity = LogHelika.GetVerbosity();
	LogHelika.SetVerbosity(ELogVerbosity::NoLogging);

	
	UHelikaManager* HelikaManager = NewObject<UHelikaManager>();
	UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey = "TestAPIKey";
	UHelikaLibrary::GetHelikaSettings()->GameId = "ValidGameId";

	// Calling before initialization
	TestTrue("Helika SDK is not initialized", !HelikaManager->SendCustomEvent(TSharedPtr<FJsonObject>()));
	TestTrue("Helika SDK is not initialized", !HelikaManager->SendCustomEvents(TArray<TSharedPtr<FJsonObject>>()));
	
	HelikaManager->InitializeSDK();
	
	// InValid Parameters
	TestTrue("Event data cannot be null or empty" , !HelikaManager->SendCustomEvent(nullptr));
	TestTrue("Invalid Event, Does not contain 'event_type' field", !HelikaManager->SendCustomEvent(MakeShareable(new FJsonObject())));
	
	{
		TSharedPtr<FJsonObject> EventData = MakeShareable(new FJsonObject());
		EventData->SetStringField("event_type", "");
		TestTrue("Invalid Event, 'event_type' field is empty", !HelikaManager->SendCustomEvent(EventData));
	}
	
	
	// Valid Parameters
	{
		TSharedPtr<FJsonObject> EventData = MakeShareable(new FJsonObject());
		EventData->SetStringField("event_type", "helikaEvent");
	
		TestTrue("Invalid Parameter Call", HelikaManager->SendCustomEvent(EventData));
	}
	
	{
		TSharedPtr<FJsonObject> EventData = MakeShareable(new FJsonObject());
		EventData->SetStringField("event_type", "helikaEvent");
		EventData->SetStringField("String Field", "Helika");
		EventData->SetNumberField("Number Field", 1234);
		EventData->SetBoolField("Bool value", true);
	
		TestTrue("Invalid Parameter Call", HelikaManager->SendCustomEvent(EventData));
	}
	
	
	
	
	//Testing SendCustomEvents
	
	// InValid Parameters
	TestTrue("Event data cannot be null or empty" , !HelikaManager->SendCustomEvents(TArray<TSharedPtr<FJsonObject>>()));
	
	{
		TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());
		TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(EventData1);
		EventArray.Add(EventData2);
		
		TestTrue("Event Data Objects does not contain 'event_type' field", !HelikaManager->SendCustomEvents(EventArray));
	}
	
	{
		TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());		
		EventData1->SetStringField("event_type", "helikaEvent");
		TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
		EventData2->SetStringField("event_type", "");
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(EventData1);
		EventArray.Add(EventData2);
		
		TestTrue("Event Data Objects contain 'event_type' field as empty", !HelikaManager->SendCustomEvents(EventArray));
	}
	
	// Valid Parameters
	{
		TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());		
		EventData1->SetStringField("event_type", "helikaEvent1");
		TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
		EventData2->SetStringField("event_type", "helikaEvent2");
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(EventData1);
		EventArray.Add(EventData2);
		
		TestTrue("Invalid Parameter call", HelikaManager->SendCustomEvents(EventArray));	
	}
	
	{
		TSharedPtr<FJsonObject> EventData1 = MakeShareable(new FJsonObject());		
		EventData1->SetStringField("event_type", "helikaEvent1");
		EventData1->SetStringField("String Field", "Helika");
		EventData1->SetNumberField("Number Field", 1234);
		EventData1->SetBoolField("Bool value", true);
		TSharedPtr<FJsonObject> EventData2 = MakeShareable(new FJsonObject());
		EventData2->SetStringField("event_type", "helikaEvent2");
		EventData2->SetStringField("String Field", "Helika");
		EventData2->SetNumberField("Number Field", 1234);
		EventData2->SetBoolField("Bool value", true);
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(EventData1);
		EventArray.Add(EventData2);
		
		TestTrue("Invalid parameter call", HelikaManager->SendCustomEvents(EventArray));	
	}

	LogHelika.SetVerbosity(OriginalVerbosity);	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHelikaGetterSetterTest, "Helika.HelikaGetterSetterTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FHelikaGetterSetterTest::RunTest(const FString& Parameters)
{
	UHelikaManager* HelikaManager = NewObject<UHelikaManager>();
	UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey = "TestAPIKey";
	UHelikaLibrary::GetHelikaSettings()->GameId = "ValidGameId";
	
	HelikaManager->InitializeSDK();

	// User details

	HelikaManager->SetUserDetails("USER001", "mayank.rajput@chicmic.in", "0x1234567890abcdef1234567890abcdef12345678");

	UE_LOG(LogHelika, Log, TEXT("User Details : %s"), *HelikaManager->GetUserDetails()->ToJsonString());

	// App details
	
	HelikaManager->SetAppDetails("PLATFORM001", "0.0.1", "0.0.1", "STORE001", "SOURCE001");

	UE_LOG(LogHelika, Log, TEXT("App Details : %s"), *HelikaManager->GetAppDetails()->ToJsonString());

	// Pii Tracking  Note: this method already contains AppendHelikaData and AppendPiiData

	HelikaManager->SetPiiTracking(true);

	// Template Event

	TSharedPtr<FJsonObject> TemplateEvent = HelikaManager->GetTemplateEvent("session_created", "session_data_updated");
	FString JsonString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(TemplateEvent.ToSharedRef(), Writer);
	UE_LOG(LogHelika, Log, TEXT("Template Event : %s"), *JsonString);
	
	
	return true;
}



#endif