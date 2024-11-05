// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HelikaTypes.h"
#include "HelikaManager.generated.h"

struct FHelikaJsonValue;
struct FHelikaJsonObject;
/**
 * 
 */
UCLASS()
class HELIKA_API UHelikaManager : public UObject
{
	GENERATED_BODY()

private:

	UHelikaManager(): PiiTracking(false), Enabled(false), AppDetails(), UserDetails()
	{
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
	void SendEvent(FString EventName, const FHelikaJsonObject& EventProps);
	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendEvents(FString EventName, TArray<FHelikaJsonObject> EventProps) const; 

	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendCustomEvent(const FHelikaJsonObject& EventProps);
	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendCustomEvents(TArray<FHelikaJsonObject> EventProps) const;

	
	bool SendEvent(FString EventName, TSharedPtr<FJsonObject> EventProps) const;
	bool SendEvents(FString EventName, TArray<TSharedPtr<FJsonObject>> EventProps) const;
	
	bool SendCustomEvent(TSharedPtr<FJsonObject> EventProps) const;    
	bool SendCustomEvents(TArray<TSharedPtr<FJsonObject>> EventProps) const;

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

	UFUNCTION(BlueprintCallable, Category = "Helika")
	FString GetKochavaDeviceId();

	TSharedPtr<FJsonObject> CreateInstallEvent();


	UUserDetails* GetUserDetails() const;
	void SetUserDetails(const FString& InUserId, const FString& InEmail, const FString& InWalletId, bool CreateNewAnon = false);

	UAppDetails* GetAppDetails() const;
	void SetAppDetails(const FString& InPlatformId, const FString& InCAV, const FString& InSAV, const FString& InStoreId, const FString& InSourceId);

	bool GetPiiTracking() const;
	void SetPiiTracking(bool InPiiTracking);

	bool IsEnabled() const;
	void SetEnabled(bool InEnabled);

	TSharedPtr<FJsonObject> GetTemplateEvent(FString EventType, FString EventSubType = "") const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Helika|Events")
	FHelikaJsonObject GetTemplateEventAsHelikaJson(FString EventType, FString EventSubType = "");

	FString GenerateAnonId(bool bBypassStored = false);

	FDateTime AddHours(FDateTime Date, int Hours);
	FDateTime AddMinutes(FDateTime Date, int Minutes);
	void ExtendSession();
protected:
	FString BaseUrl;
	FString SessionId;
	ETelemetryLevel Telemetry = ETelemetryLevel::TL_None;
	bool bIsInitialized = false;
	FString KochavaDeviceID;

	FDateTime SessionExpiry;
	bool PiiTracking;
	bool Enabled;
	UPROPERTY()
	UAppDetails* AppDetails;
	UPROPERTY(EditAnywhere)
	UUserDetails* UserDetails;
	FString AnonId;
	
	

private:
	TSharedPtr<FJsonObject> AppendAttributesToJsonObject(TSharedPtr<FJsonObject> JsonObject) const;
	TSharedPtr<FJsonObject> AppendAttributesToJsonObject(const FString& EventName, const TSharedPtr<FJsonObject>& JsonObject) const;
	void CreateSession() const;
	void SendHTTPPost(const FString& Url, const FString& Data) const;
	static void ProcessEventSentSuccess(const FString& Data);
	static void ProcessEventSentError(const FString& Data);
	static void EndEditorSession(bool bIsSimulating);
	FString GenerateKochavaDeviceID();

	UFUNCTION(BlueprintCallable, Category = "Helika")
	void InitializeTracking();

	TSharedPtr<FJsonObject> AppendHelikaData() const;
	TSharedPtr<FJsonObject> AppendPiiData(TSharedPtr<FJsonObject> HelikaData);
	
	void EndSession() const;
};
