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

	UFUNCTION(BlueprintCallable, Category = "Helika")
	void InitializeSDK();
	void DeinitializeSDK();

	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendEvent(FString EventName, const FHelikaJsonObject& EventProps);
	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendEvents(FString EventName, TArray<FHelikaJsonObject> EventProps) const; 

	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendCustomEvent(const FHelikaJsonObject& EventProps);
	UFUNCTION(BlueprintCallable, Category="Helika|Events")
	void SendCustomEvents(TArray<FHelikaJsonObject> EventProps) const;

	
	void SendEvent(FString EventName, TSharedPtr<FJsonObject> EventProps) const;
	void SendEvents(FString EventName, TArray<TSharedPtr<FJsonObject>> EventProps) const;
	
	void SendCustomEvent(TSharedPtr<FJsonObject> EventProps) const;    
	void SendCustomEvents(TArray<TSharedPtr<FJsonObject>> EventProps) const;

	// Sets the player ID
	UFUNCTION(BlueprintCallable, Category="Helika")
	void SetPlayerId(const FString& InPlayerId);
	
	// Get the player ID
	UFUNCTION(BlueprintPure, Category="Helika")
	FString GetPlayerId();

	// Set weather to print events to console or not
	UFUNCTION(BlueprintCallable, Category="Helika")
	void SetPrintToConsole(bool bInPrintEventsToConsole);

protected:
	FString BaseUrl;
	FString SessionId;
	ETelemetryLevel Telemetry = ETelemetryLevel::TL_None;
	bool bIsInitialized = false;

private:
	TSharedPtr<FJsonObject> AppendAttributesToJsonObject(TSharedPtr<FJsonObject> JsonObject) const;
	TSharedPtr<FJsonObject> AppendAttributesToJsonObject(const FString& EventName, const TSharedPtr<FJsonObject>& JsonObject) const;
	void CreateSession() const;
	void SendHTTPPost(const FString& Url, const FString& Data) const;
	static void ProcessEventTrackResponse(const FString& Data);
	static void EndSession(bool bIsSimulating);
};
