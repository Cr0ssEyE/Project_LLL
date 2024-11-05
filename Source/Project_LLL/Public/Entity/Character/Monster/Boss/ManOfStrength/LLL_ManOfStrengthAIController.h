// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonsterAIController.h"
#include "LLL_ManOfStrengthAIController.generated.h"

UCLASS()
class PROJECT_LLL_API ALLL_ManOfStrengthAIController : public ALLL_BossMonsterAIController
{
	GENERATED_BODY()
	
protected:
	virtual void Tick(float DeltaTime) override;
};
