// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HelikaTypes.h"
#include "HelikaSettings.generated.h"
/**
 * 
 */
UCLASS(Config = Engine, DefaultConfig)
class HELIKA_API UHelikaSettings : public UObject
{
	GENERATED_BODY()

public:
	/// Helika API key received from the dashboard
	UPROPERTY(Config, EditAnywhere, Category = "Helika")
	FString HelikaAPIKey;

	/// Game ID
	UPROPERTY(Config, EditAnywhere, Category = "Helika")
	FString GameId;

	/// Player ID
	UPROPERTY(Config, EditAnywhere, Category = "Helika")
	FString PlayerId;

	/// Helika environment to be used while sending API calls
	UPROPERTY(Config, EditAnywhere, Category = "Helika")
	EHelikaEnvironment HelikaEnvironment = EHelikaEnvironment::HE_Localhost;

	/// Telemetry level, Allows to send selective data based on telemetry level
	UPROPERTY(Config, EditAnywhere, Category = "Helika")
	ETelemetryLevel Telemetry = ETelemetryLevel::TL_None;

	/// Print Events to console
	UPROPERTY(Config, EditAnywhere, Category = "Helika")
	bool bPrintEventsToConsole = true;

	UPROPERTY(Config, VisibleAnywhere, Category = "Helika")
	FString SDKName = "Unreal";
	
	UPROPERTY(Config, VisibleAnywhere, Category = "Helika")
	FString SDKVersion = "0.4.0";

	UPROPERTY(Config, VisibleAnywhere, Category = "Helika")
	FString SDKClass = "HelikaSubsystem";
};
