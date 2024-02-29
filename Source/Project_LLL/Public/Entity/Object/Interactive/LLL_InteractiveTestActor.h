// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_InteractiveObject.h"
#include "LLL_InteractiveTestActor.generated.h"

UCLASS()
class PROJECT_LLL_API ALLL_InteractiveTestActor : public ALLL_InteractiveObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_InteractiveTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void InteractiveEvent() override;
	
};
