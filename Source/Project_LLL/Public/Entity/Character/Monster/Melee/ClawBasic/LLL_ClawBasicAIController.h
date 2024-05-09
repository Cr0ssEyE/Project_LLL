// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Monster/Melee/Base/LLL_MeleeMonsterAIController.h"
#include "LLL_ClawBasicAIController.generated.h"

UCLASS()
class PROJECT_LLL_API ALLL_ClawBasicAIController : public ALLL_MeleeMonsterAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_ClawBasicAIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
