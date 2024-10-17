// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HelikaTypes.h"
#include "GameFramework/Actor.h"
#include "Containers/UnrealString.h"
#include "HelikaActor.generated.h"

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
    FString ApiKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    FString GameId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    FString PlayerId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    EHelikaEnvironment HelikaEnvironment = EHelikaEnvironment::HE_Localhost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    ETelemetryLevel Telemetry = ETelemetryLevel::TL_All;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helika)
    bool bPrintEventsToConsole = false;

    void Init(const FString& InApiKey, const FString& InGameId, EHelikaEnvironment InHelikaEnvironment, ETelemetryLevel InTelemetryLevel = ETelemetryLevel::TL_All, bool bInPrintEventsToConsole = false);

    // Set weather to print events to console or not
    UFUNCTION(BlueprintCallable, Category = "Helika")
    void SetPrintToConsole(bool bInPrintEventsToConsole);

    // Get the player ID
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
    FString GetPlayerId();

    // Sets the player ID
    UFUNCTION(BlueprintCallable, Category = "Helika")
    void SetPlayerID(FString InPlayerID);

    // Get Current Platform type (Windows, Mac, Android, IOS, Linux, etc.) return as enum
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
    static EPlatformType GetPlatformType();

    // Get Current Platform name (Windows, Mac, Android, IOS, Linux, etc.) return in FString
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
    static FString GetPlatformName();

    // Get the device unique identifier
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
    static FString GetDeviceUniqueIdentifier();

    void SendCustomEvent(TSharedPtr<FJsonObject> EventProps) const;
    
    void SendCustomEvents(TArray<TSharedPtr<FJsonObject>> EventProps) const;

private:
    FString SDKName = "Unreal";

    FString SDKVersion = "0.1.1";

    FString SDKClass = "HelikaActor";

    static void ProcessEventTrackResponse(const FString& Data);

    void AppendAttributesToJsonObject(const TSharedPtr<FJsonObject>& JsonObject) const;

    void CreateSession() const;
    
    void SendHTTPPost(const FString& Url, const FString& Data) const;

    static void AddIfNull(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue);

    static void AddOrReplace(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue);
    
    static FString ConvertUrl(const EHelikaEnvironment InHelikaEnvironment);
    
    // Get the device type (Desktop, Mobile, Console, etc.)
    static FString GetDeviceType();
    
    static FString GetDeviceProcessorType();

protected:
    
    FString BaseUrl;
    
    FString SessionId;
    
    FString DeviceId;
    
    bool bIsInitialized = false;
};
