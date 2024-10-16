// Fill out your copyright notice in the Description page of Project Settings.


#include "HelikaSubsystem.h"

#include "HelikaDefines.h"

UHelikaSubsystem::UHelikaSubsystem()
	: UEngineSubsystem()
{
}

void UHelikaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogHelika, Log, TEXT("Helika subsystem initialized"));
}

void UHelikaSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UE_LOG(LogHelika, Log, TEXT("Helika subsystem deinitialized"));
}

