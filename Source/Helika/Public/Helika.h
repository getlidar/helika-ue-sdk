// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class UHelikaSubsystem;
class UHelikaSettings;

class FHelikaModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /// Singleton like access to this module's interface.
    /// @warning Be aware of calling this during shutdown phase, module might have been unloaded already.
    /// @return Returns singleton instance, loading the module on demand if needed
    static inline FHelikaModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FHelikaModule>("Helika");
    }

    /// Checks to see if this module is loaded and ready. It is only valid to call Get() if IsAvailable() returns true.
    /// @return True if the module is loaded and ready to use
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("Helika");
    }

    /// Getter for internal settings object to support runtime configurations changes
    UHelikaSettings* GetSettings() const;

protected:
    /// Module Settings : will be editable in Unreal Editor and saved in DefaultEngine.ini
    UHelikaSettings* ModuleSettings = nullptr;
};
