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

UENUM(BlueprintType)
enum class EDisableDataSettings : uint8
{
	DDS_None = 0 UMETA(DisplayName = "None"),
	DDS_DeviceInfo = 1 UMETA(DisplayName = "Device Info"),
	DDS_IpInfo = 2 UMETA(DisplayName = "IP Info"),
	DDS_OsInfo = 3 UMETA(DisplayName = "OS Info"),
	DDS_All = 4 UMETA(DisplayName = "All"),
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

	void Initialize(FString InPlatformId, FString InClientAppVersion, FString InServerAppVersion, FString InStoreId, FString InSourceId);

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
	FString UserId;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString Email;
	
	UPROPERTY(BlueprintReadWrite, Category="Helika|AppDetails")
	FString WalletId;

	void Initialize(FString InUserId, FString InEmail, FString InWalletId);
	
	TSharedPtr<FJsonObject> ToJson() const;

	UFUNCTION(BlueprintCallable, Category="Helika|AppDetails")
	FString ToJsonString() const;

	UFUNCTION(BlueprintCallable, Category="Helika|AppDetails")
	FHelikaJsonObject ToHelikaJsonObject() const;
};



