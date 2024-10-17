// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HelikaTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelikaLibrary.generated.h"

enum class EHelikaEnvironment : uint8;
class UHelikaSettings;
/**
 * Useful methods that are independent and static
 */
UCLASS()
class HELIKA_API UHelikaLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Helika|Common")
	static UHelikaSettings* GetHelikaSettings();

	static FString ConvertUrl(const EHelikaEnvironment InHelikaEnvironment);

	static FString CreateNewGuid();

	// Get Current Platform name (Windows, Mac, Android, IOS, Linux, etc.) return in FString
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
	static FString GetPlatformName();

	// Get Current Platform type (Windows, Mac, Android, IOS, Linux, etc.) return as enum
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
	static EPlatformType GetPlatformType();

	// Get the device unique identifier
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
	static FString GetDeviceUniqueIdentifier();

	// Get the device type (Desktop, Mobile, Console, etc.)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
	static FString GetDeviceType();
	
	// Get the device processor name
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Helika")
	static FString GetDeviceProcessor();

public:

	static void AddIfNull(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue);
	static void AddOrReplace(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue);

};