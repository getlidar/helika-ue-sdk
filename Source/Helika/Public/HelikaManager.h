// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HelikaTypes.h"
#include "HelikaManager.generated.h"

/**
 * 
 */
UCLASS()
class HELIKA_API UHelikaManager : public UObject
{
	GENERATED_BODY()

private:

	UHelikaManager(){};

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

	void InitializeSDK();
	void DeinitializeSDK();

	void SendCustomEvent(TSharedPtr<FJsonObject> EventProps) const;    
	void SendCustomEvents(TArray<TSharedPtr<FJsonObject>> EventProps) const;

	// Sets the player ID
	void SetPlayerId(const FString& InPlayerId);
	
	// Get the player ID
	FString GetPlayerId();

	// Set weather to print events to console or not
	void SetPrintToConsole(bool bInPrintEventsToConsole);

protected:
	FString BaseUrl;
	FString SessionId;
	ETelemetryLevel Telemetry = ETelemetryLevel::TL_None;
	bool bIsInitialized = false;

private:
	void AppendAttributesToJsonObject(const TSharedPtr<FJsonObject>& JsonObject) const;
	void CreateSession() const;
	void SendHTTPPost(const FString& Url, const FString& Data) const;
	static void ProcessEventTrackResponse(const FString& Data);
	static void EndSession(bool bIsSimulating);
};
