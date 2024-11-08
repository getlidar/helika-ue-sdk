// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HelikaActor.generated.h"

class UHelikaManager;

UCLASS()
class HELIKA_API AHelikaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHelikaActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UHelikaManager* HelikaManager = nullptr;

};
