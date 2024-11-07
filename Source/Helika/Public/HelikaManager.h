// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HelikaJsonLibrary.h"
#include "HelikaTypes.h"
#include "HelikaManager.generated.h"

struct FHelikaJsonValue;
struct FHelikaJsonObject;
/**
 * 
 */
UCLASS(BlueprintType)
class HELIKA_API UHelikaManager : public UObject
{
	GENERATED_BODY()

private:

	UHelikaManager():AppDetails(MakeShareable(new FJsonObject())), UserDetails(MakeShareable(new FJsonObject()))
	{
		AppDetails->SetField("platform_id", nullptr);
		AppDetails->SetField("client_app_version", nullptr);
		AppDetails->SetField("server_app_version", nullptr);
		AppDetails->SetField("store_id", nullptr);
		AppDetails->SetField("source_id", nullptr);

		UserDetails->SetField("user_id", nullptr);
		UserDetails->SetField("email", nullptr);
		UserDetails->SetField("wallet", nullptr);
	};

	static UHelikaManager* Instance;

public:
	static inline UHelikaManager* Get()
	{
		if(!Instance)
		{
			Instance = NewObject<UHelikaManager>();
			Instance->AddToRoot();
		}
		return Instance;
	}

	UFUNCTION(BlueprintCallable, Category = "Helika")
	void InitializeSDK();
	UFUNCTION(BlueprintCallable, Category = "Helika")
	void DeinitializeSDK();

	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendEvent(const FHelikaJsonObject& EventProps);
	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendEvents(TArray<FHelikaJsonObject> EventProps); 

	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendUserEvent(const FHelikaJsonObject& EventProps);
	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendUserEvents(TArray<FHelikaJsonObject> EventProps);

	
	bool SendEvent(TSharedPtr<FJsonObject> EventProps);
	bool SendEvents(TArray<TSharedPtr<FJsonObject>> EventProps);
	
	bool SendUserEvent(TSharedPtr<FJsonObject> EventProps);    
	bool SendUserEvents(TArray<TSharedPtr<FJsonObject>> EventProps);

	// Sets the player ID
	UE_DEPRECATED(0.1.1, "SetPlayerId() is deprecated. Please use SetUserDetails() instead")
	UFUNCTION(BlueprintCallable, Category="Helika")
	void SetPlayerId(const FString& InPlayerId);
	
	// Get the player ID
	UE_DEPRECATED(0.1.1, "GetPlayerId() is deprecated. Please use GetUserDetails() instead")
	UFUNCTION(BlueprintPure, Category="Helika")
	FString GetPlayerId();

	// Set weather to print events to console or not
	UFUNCTION(BlueprintCallable, Category="Helika")
	void SetPrintToConsole(bool bInPrintEventsToConsole);

	UFUNCTION(BlueprintCallable, Category = "Helika")
	bool IsSDKInitialized();

	UFUNCTION(BlueprintCallable, Category = "Helika")
	FString GetSessionId() const;

	TSharedPtr<FJsonObject> GetUserDetails();
	void SetUserDetails(TSharedPtr<FJsonObject> InUserDetails, bool bCreateNewAnonId = false);

	UFUNCTION(BlueprintPure, Category="Helika")
	FHelikaJsonObject GetUserDetailsAsJson();
	UFUNCTION(BlueprintCallable, Category="Helika")
	void SetUserDetails(const FHelikaJsonObject& InUserDetails, bool bCreateNewAnonId = false);

	TSharedPtr<FJsonObject> GetAppDetails();
	void SetAppDetails(const TSharedPtr<FJsonObject>& InAppDetails);

	UFUNCTION(BlueprintPure, Category="Helika")
	FHelikaJsonObject GetAppDetailsAsJson();
	UFUNCTION(BlueprintCallable, Category="Helika")
	void SetAppDetails(const FHelikaJsonObject& InAppDetails);

	UFUNCTION(BlueprintPure, Category="Helika")
	bool GetPIITracking() const;
	UFUNCTION(BlueprintCallable, Category="Helika")
	void SetPIITracking(bool bInPiiTracking, bool bSendPiiTrackingEvent = false);
	
protected:
	FString BaseUrl;
	FString SessionId;
	ETelemetryLevel Telemetry = ETelemetryLevel::TL_None;
	bool bIsInitialized = false;

	bool bPiiTracking = false;
	FString AnonymousId;

	TSharedPtr<FJsonObject> AppDetails;
	TSharedPtr<FJsonObject> UserDetails;

private:
	TSharedPtr<FJsonObject> AppendAttributesToJsonObject(TSharedPtr<FJsonObject> JsonObject, bool bIsUserEvent);
	void CreateSession();
	void SendHTTPPost(const FString& Url, const FString& Data) const;
	static void ProcessEventTrackResponse(const FString& Data);
	static void EndSession(bool bIsSimulating);

	FString GenerateAnonymousId(FString Seed, bool bCreateNewAnonId = false);

	TSharedPtr<FJsonObject> GetTemplateEvent(const FString& EventType, const FString& EventSubType) const;
	void AppendHelikaData(const TSharedPtr<FJsonObject>& GameEvent) const;
	void AppendUserDetails(const TSharedPtr<FJsonObject>& GameEvent) const;
	void AppendAppDetails(const TSharedPtr<FJsonObject>& GameEvent) const;
	void AppendPIITracking(const TSharedPtr<FJsonObject>& GameEvent);
};
