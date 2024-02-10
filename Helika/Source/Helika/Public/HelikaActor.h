// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Misc/Guid.h"
#include "Misc/DateTime.h"
#include "Containers/UnrealString.h"
#include "HelikaActor.generated.h"

UENUM(BlueprintType)
enum class HelikaEnvironment : uint8 {
	Localhost,
	Develop,
	Production
};

USTRUCT(BlueprintType)
struct FHEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString game_id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString event_type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString>  event;

	UPROPERTY()
	FString created_at;
};

USTRUCT(BlueprintType)
struct FHSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FHEvent> events;
};

UCLASS()
class HELIKA_API AHelikaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AHelikaActor();

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
	FString apiKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
	FString gameId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
	FString playerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
	HelikaEnvironment helikaEnv;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
	bool sendingEvents = false;

	void Init(FString apiKeyIn, FString gameIdIN, HelikaEnvironment env, bool enabled = false);

	UFUNCTION(BlueprintCallable, Category = "Helika")
	void SetPlayerId(FString playerId);
	
	UFUNCTION(BlueprintCallable, Category = "Helika")
	void SetEnableEvents(bool enabled);

	UFUNCTION(BlueprintCallable, Category = "Helika")
	void SendEvent(FHSession helikaEvents);

private:        
	FString _helikaApiKey;

	void SendHTTPPost(FString url, FString data);

	void ProcessEventTrackResponse(FString data);

	void CreateSession();

	FString ConvertUrl(HelikaEnvironment baseUrl);

protected:
	FString _baseUrl;

	FString _gameId;

	FString _sessionID;

	FString _playerId;

	FString _deviceId;

	bool _isInitialized = false;

	bool _enabled = false;
};
