// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/HelikaActor.h"

#include "HelikaLibrary.h"
#include "HelikaManager.h"
#include "HelikaSettings.h"

// Sets default values
AHelikaActor::AHelikaActor()
{
}

// The HelikaManager is a singleton, so it only needs to be initalized once
// at the start of the game. Every subsequent use of the HelikaManager can simply
// use the UHelikaManager::Get()
// Recommend to Initialize the SDK with Game Instance Init() method

// Called when the game starts or when spawned
void AHelikaActor::BeginPlay()
{
	Super::BeginPlay();

	HelikaManager = UHelikaManager::Get();

	// In case to set the api key, game id and player id use following sample
	// UHelikaLibrary::GetHelikaSettings()->HelikaAPIKey = "APIKEY";
	// UHelikaLibrary::GetHelikaSettings()->GameId = "GAMEID";
	// HelikaManager->SetPlayerId("PLAYER_ID_TEST");

	{
		TSharedPtr<FJsonObject> UserDetails = MakeShareable(new FJsonObject());
		UserDetails->SetStringField("user_id", HelikaManager->GetPlayerId());
		UserDetails->SetStringField("email", "test@gmail.com");
		UserDetails->SetStringField("wallet", "0x8540507642419A0A8Af94Ba127F175dA090B58B0");
		
		HelikaManager->SetUserDetails(UserDetails);
		
		TSharedPtr<FJsonObject> AppDetails = MakeShareable(new FJsonObject());
		AppDetails->SetStringField("platform_id", UHelikaLibrary::GetPlatformName());
		AppDetails->SetStringField("client_app_version", "0.1.1");
		AppDetails->SetObjectField("server_app_version", nullptr);
		AppDetails->SetStringField("store_id", "EpicGames");
		AppDetails->SetObjectField("source_id", nullptr);
		
		HelikaManager->SetAppDetails(AppDetails);
		
		// Initializing SDK only required once
		HelikaManager->InitializeSDK();
	}

	{
		// This is an example of sending a single user event
		const TSharedPtr<FJsonObject> PlayerKillEvent = MakeShareable(new FJsonObject());
		PlayerKillEvent->SetStringField("event_type", "player_event");
		const TSharedPtr<FJsonObject> PlayerKillSubEvent = MakeShareable(new FJsonObject());
		PlayerKillSubEvent->SetStringField("event_sub_type", "player_killed");
		PlayerKillSubEvent->SetNumberField("user_id", 10);
		PlayerKillSubEvent->SetNumberField("damage_amount", 40);
		PlayerKillSubEvent->SetNumberField("bullets_fired", 15);
		PlayerKillSubEvent->SetStringField("map", "arctic");
		PlayerKillEvent->SetObjectField("event", PlayerKillSubEvent);
	
		HelikaManager->SendUserEvent(PlayerKillEvent);		
	}
	
	{
		// This is an example of sending multiple events at once 
		const TSharedPtr<FJsonObject> Event1 = MakeShareable(new FJsonObject());
		Event1->SetStringField("event_type", "bomb_event");
		const TSharedPtr<FJsonObject> Event1SubEvent = MakeShareable(new FJsonObject());
		Event1SubEvent->SetStringField("event_sub_type", "bomb_planted");
		Event1SubEvent->SetStringField("map", "arctic");
		Event1SubEvent->SetStringField("team", "counter-terrorists");
		Event1->SetObjectField("event", Event1SubEvent);
	
		const TSharedPtr<FJsonObject> Event2 = MakeShareable(new FJsonObject());
		Event2->SetStringField("event_type", "bomb_event");
		const TSharedPtr<FJsonObject> Event2SubEvent = MakeShareable(new FJsonObject());
		Event2SubEvent->SetStringField("event_sub_type", "bomb_diffused");
		Event2SubEvent->SetStringField("map", "arctic");
		Event2SubEvent->SetStringField("team", "counter-terrorists");
		Event2SubEvent->SetNumberField("duration", 10210.121);
		Event2->SetObjectField("event", Event2SubEvent);
	
		TArray<TSharedPtr<FJsonObject>> EventArray;
		EventArray.Add(Event1);
		EventArray.Add(Event2);
		HelikaManager->SendUserEvents(EventArray);
	}
	
	{
		// This is an example of a non-user event. For Non-user events, we don't automatically append user information
		const TSharedPtr<FJsonObject> WinEvent = MakeShareable(new FJsonObject());
		WinEvent->SetStringField("event_type", "game_finished");
		const TSharedPtr<FJsonObject> WinSubEvent = MakeShareable(new FJsonObject());
		WinSubEvent->SetStringField("event_sub_type", "win_results");
		WinSubEvent->SetStringField("winner", "counter-terrorists");
		WinSubEvent->SetStringField("map", "arctic");
		WinEvent->SetObjectField("event", WinSubEvent);
	
		HelikaManager->SendEvent(WinEvent);		
	}

	{
		// Log out user
		TSharedPtr<FJsonObject> UpdatedUserDetails = MakeShareable(new FJsonObject());
		UpdatedUserDetails->SetObjectField("user_id", nullptr);
		HelikaManager->SetUserDetails(UpdatedUserDetails);

		// Clear and Reset User Details
		const TSharedPtr<FJsonObject> LogoutEvent = MakeShareable(new FJsonObject());
		LogoutEvent->SetStringField("event_type", "logout");
		const TSharedPtr<FJsonObject> LogoutSubEvent = MakeShareable(new FJsonObject());
		LogoutSubEvent->SetStringField("event_sub_type", "user_logged_out");
		LogoutEvent->SetObjectField("event", LogoutSubEvent);
		
		HelikaManager->SendUserEvent(LogoutEvent);		
	}

	{
		// Set New User info
		TSharedPtr<FJsonObject> UpdatedUserDetails = MakeShareable(new FJsonObject());
		UpdatedUserDetails->SetStringField("user_id", "new_player_id");
		HelikaManager->SetUserDetails(UpdatedUserDetails);

		// Clear and Reset User Details
		const TSharedPtr<FJsonObject> LoginEvent = MakeShareable(new FJsonObject());
		LoginEvent->SetStringField("event_type", "login");
		const TSharedPtr<FJsonObject> LoginSubEvent = MakeShareable(new FJsonObject());
		LoginSubEvent->SetStringField("event_sub_type", "user_logged_in");
		LoginEvent->SetObjectField("event", LoginSubEvent);
	
		HelikaManager->SendUserEvent(LoginEvent);
	}
	
	{
		const TSharedPtr<FJsonObject> AppDetails = HelikaManager->GetAppDetails();
		AppDetails->SetStringField("source_id", "google_ads");
		AppDetails->SetStringField("client_app_version", "0.0.3");
		HelikaManager->SetAppDetails(AppDetails);
	
		const TSharedPtr<FJsonObject> UpgradeEvent = MakeShareable(new FJsonObject());
		UpgradeEvent->SetStringField("event_type", "upgrade");
		const TSharedPtr<FJsonObject> UpgradeSubEvent = MakeShareable(new FJsonObject());
		UpgradeSubEvent->SetStringField("event_sub_type", "upgrade_finished");
		UpgradeEvent->SetObjectField("event", UpgradeSubEvent);
	
		HelikaManager->SendUserEvent(UpgradeEvent);
	}
	
	{
		// Setting PII Tracking
		HelikaManager->SetPIITracking(true, true);
	}
}
