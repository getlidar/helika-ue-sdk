// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "HelikaSubsystem.generated.h"
/**
 * 
 */
UCLASS()
class HELIKA_API UHelikaSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	UHelikaSubsystem();

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem
	
};
