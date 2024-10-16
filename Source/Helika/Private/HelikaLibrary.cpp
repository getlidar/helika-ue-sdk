// Fill out your copyright notice in the Description page of Project Settings.


#include "HelikaLibrary.h"
#include "Helika.h"
#include "HelikaTypes.h"


UHelikaSettings* UHelikaLibrary::GetHelikaSettings()
{
	return FHelikaModule::Get().GetSettings();
}

FString UHelikaLibrary::ConvertUrl(const EHelikaEnvironment InHelikaEnvironment)
{
	switch (InHelikaEnvironment)
	{
	case EHelikaEnvironment::HE_Production:
		return "https://api.helika.io/v1";
	case EHelikaEnvironment::HE_Develop:
		return "https://api-stage.helika.io/v1";
	case EHelikaEnvironment::HE_Localhost:
		return "http://localhost:8181/v1";
	default:
		return "http://localhost:8181/v1";
	}
}

FString UHelikaLibrary::CreateNewGuid()
{
	return FGuid::NewGuid().ToString();
}

FString UHelikaLibrary::GetPlatformName()
{
#if PLATFORM_WINDOWS
    return FString(TEXT("Windows"));
#elif PLATFORM_IOS
    return FString(TEXT("IOS"));
#elif PLATFORM_MAC
    return FString(TEXT("Mac"));
#elif PLATFORM_ANDROID
    return FString(TEXT("Android"));
#elif PLATFORM_LINUX
    return FString(TEXT("Linux"));
#elif PLATFORM_CONSOLE
    return FString(TEXT("Console"));
#else
    ensureMsgf(false, TEXT("Platform unknown"));
    return FString(TEXT("Unknown"));
#endif
}

EPlatformType UHelikaLibrary::GetPlatformType()
{
#if PLATFORM_WINDOWS
    return EPlatformType::PT_WINDOWS;
#elif PLATFORM_IOS
    return EPlatformType::PT_IOS;
#elif PLATFORM_MAC
    return EPlatformType::PT_MAC;
#elif PLATFORM_ANDROID
    return EPlatformType::PT_ANDROID;
#elif PLATFORM_LINUX
    return EPlatformType::PT_LINUX;
#elif PLATFORM_CONSOLE
    return EPlatformType::PT_CONSOLE;
#else
    ensureMsgf(false, TEXT("Platform unknown"));
    return EPlatformType::PT_UNKNOWN;
#endif
}

///
/// In case of Windows and Mac we have access to operating system ID
/// In case of Android and iOS we have access to device ID
/// @return
FString UHelikaLibrary::GetDeviceUniqueIdentifier()
{
#if PLATFORM_WINDOWS || PLATFORM_MAC
    return FPlatformMisc::GetOperatingSystemId();
#elif PLATFORM_ANDROID || PLATFORM_IOS
    return FPlatformMisc::GetDeviceId();
#else
    return FPlatformMisc::GetDeviceId();
#endif
}

FString UHelikaLibrary::GetDeviceType()
{   switch (GetPlatformType())
    {
    case EPlatformType::PT_IOS :
    case EPlatformType::PT_ANDROID :
        {
            return "Mobile";
        }
    case EPlatformType::PT_WINDOWS :
    case EPlatformType::PT_MAC :
    case EPlatformType::PT_LINUX :
        {
            return "Desktop";
        }
    case EPlatformType::PT_CONSOLE :
        {
            return "Console";
        }
    case EPlatformType::PT_DEFAULT :
    case EPlatformType::PT_UNKNOWN :
        {
            return "Unknown";            
        }
    }
        return FString();
}

FString UHelikaLibrary::GetDeviceProcessor()
{
    switch (GetPlatformType())
    {
    case EPlatformType::PT_IOS :
    case EPlatformType::PT_ANDROID :
        {
            return FPlatformMisc::GetCPUChipset();
        }
    case EPlatformType::PT_WINDOWS :
    case EPlatformType::PT_MAC :
    case EPlatformType::PT_LINUX :
    case EPlatformType::PT_CONSOLE :
        {
            return FPlatformMisc::GetCPUBrand();
        }
    case EPlatformType::PT_DEFAULT :
    case EPlatformType::PT_UNKNOWN :
        {
            return FString();            
        }
    }
    return FString();
}

void UHelikaLibrary::AddIfNull(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue)
{
    if(!HelikaEvent->HasField(Key))
    {
        HelikaEvent->SetStringField(Key, NewValue);
    }
}

void UHelikaLibrary::AddOrReplace(const TSharedPtr<FJsonObject>& HelikaEvent, const FString& Key, const FString& NewValue)
{
    if(HelikaEvent->HasField(Key))
    {
        HelikaEvent->SetStringField(Key, NewValue);
    }
    else
    {
        HelikaEvent->SetStringField(Key, NewValue);
    }
}





