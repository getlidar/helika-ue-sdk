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
enum class HelikaEnvironment : uint8
{
    Localhost,
    Develop,
    Production
};

UENUM(BlueprintType)
enum class EPlatformType : uint8
{
    PT_DEFAULT UMETA(DisplayName = "Default"),
    PT_WINDOWS UMETA(DisplayName = "Windows"),
    PT_MAC UMETA(DisplayName = "Mac"),
    PT_LINUX UMETA(DisplayName = "Linux"),
    PT_IOS UMETA(DisplayName = "IOS"),
    PT_ANDROID UMETA(DisplayName = "Android"),
    PT_UNKNOWN UMETA(DisplayName = "Unknown")
};

UENUM(BlueprintType)
enum class TelemetryLevel : uint8
{
    None = 0 UMETA(DisplayName = "None"),
    TelemetryOnly = 100 UMETA(DisplayName = "TelemetryOnly"),
    All = 200 UMETA(DisplayName = "All"),
};

USTRUCT(BlueprintType)
struct FHEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    FString game_id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    FString event_type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    TMap<FString, FString> event;

    UPROPERTY()
    FString created_at;
};

USTRUCT(BlueprintType)
struct FHSession
{
    GENERATED_BODY()

    UPROPERTY()
    FString id;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    TArray<FHEvent> events;
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
    TelemetryLevel telemetry = TelemetryLevel::All;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    bool printEventsToConsole = false;

    void Init(FString apiKeyIn, FString gameIdIN, HelikaEnvironment env, TelemetryLevel telemetryLevel = TelemetryLevel::All, bool isPrintEventsToConsole = false);

    UFUNCTION(BlueprintCallable, Category = "Helika")
    void SendEvent(FHSession helikaEvents);

    // Sets the player ID
    UFUNCTION(BlueprintCallable, Category = "Helika")
    void SetPlayerID(FString InPlayerID);

    // Get Current Platform type (Windows, Mac, Android, IOS, Linux, etc.) return as enum
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
    EPlatformType GetPlatformType();

    // Get Current Platform name (Windows, Mac, Android, IOS, Linux, etc.) return in FString
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
    FString GetPlatformName();

    // Get the device unique identifier
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
    FString GetDeviceUniqueIdentifier();

private:
    FString _helikaApiKey;

    FString _sdk_name = "Unreal";

    FString _sdk_version = "0.1.1";

    FString _sdk_class = "HelikaActor";

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

    bool _printEventsToConsole = false;

    TelemetryLevel _telemetry = TelemetryLevel::All;
};
