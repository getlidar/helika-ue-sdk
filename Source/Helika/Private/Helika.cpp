// Copyright Epic Games, Inc. All Rights Reserved.

#include "Helika.h"

#include "HelikaDefines.h"
#include "HelikaSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FHelikaModule"

void FHelikaModule::StartupModule()
{
	ModuleSettings = NewObject<UHelikaSettings>(GetTransientPackage(), "HelikaSettings", RF_Standalone);
	ModuleSettings->AddToRoot();

	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Helika",
			LOCTEXT("RuntimeSettingsName", "Helika"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Helika plugin settings"),
			ModuleSettings);
	}

	UE_LOG(LogHelika, Log, TEXT("Helika module started"))
}

void FHelikaModule::ShutdownModule()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Helika");
	}

	if(!GExitPurge)
	{
		ModuleSettings->RemoveFromRoot();
	}
	else
	{
		ModuleSettings = nullptr;
	}
}

UHelikaSettings* FHelikaModule::GetSettings() const
{
	check(ModuleSettings);
	return ModuleSettings;
}

DEFINE_LOG_CATEGORY(LogHelika);

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FHelikaModule, Helika)