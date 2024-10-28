// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HelikaTypes.generated.h"

struct FHelikaJsonObject;
/// Helika API Environment
UENUM(BlueprintType)
enum class EHelikaEnvironment : uint8
{
	HE_Localhost UMETA(DisplayName = "Localhost"),
	HE_Develop UMETA(DisplayName = "Develop"),
	HE_Production UMETA(DisplayName = "Production")
};

/// Helika Telemetry Level. Weather to send complete device details or not
UENUM(BlueprintType)
enum class ETelemetryLevel : uint8
{
	TL_None = 0 UMETA(DisplayName = "None"),
	TL_TelemetryOnly = 100 UMETA(DisplayName = "TelemetryOnly"),
	TL_All = 200 UMETA(DisplayName = "All"),
};

/// Platform Type
UENUM(BlueprintType)
enum class EPlatformType : uint8
{
	PT_DEFAULT UMETA(DisplayName = "Default"),
	PT_WINDOWS UMETA(DisplayName = "Windows"),
	PT_MAC UMETA(DisplayName = "Mac"),
	PT_LINUX UMETA(DisplayName = "Linux"),
	PT_IOS UMETA(DisplayName = "IOS"),
	PT_ANDROID UMETA(DisplayName = "Android"),
	PT_CONSOLE UMETA(DisplayName = "Console"),
	PT_UNKNOWN UMETA(DisplayName = "Unknown")
};

UCLASS(BlueprintType)
class HELIKA_API UAppDetails : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString PlatformId;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString ClientAppVersion;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString ServerAppVersion;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString StoreId;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString SourceId;

	TSharedPtr<FJsonObject> ToJson() const;

	UFUNCTION(BlueprintCallable, Category="Helika|AppDetails")
	FString ToJsonString() const;

	UFUNCTION(BlueprintCallable, Category="Helika|AppDetails")
	FHelikaJsonObject ToHelikaJsonObject() const;
};

UCLASS(BlueprintType)
class HELIKA_API UUserDetails : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString PlatformId;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString ClientAppVersion;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString ServerAppVersion;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString StoreId;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString SourceId;

	TSharedPtr<FJsonObject> ToJson() const;

	UFUNCTION(BlueprintCallable, Category="Helika|AppDetails")
	FString ToJsonString() const;

	UFUNCTION(BlueprintCallable, Category="Helika|AppDetails")
	FHelikaJsonObject ToHelikaJsonObject() const;
};



