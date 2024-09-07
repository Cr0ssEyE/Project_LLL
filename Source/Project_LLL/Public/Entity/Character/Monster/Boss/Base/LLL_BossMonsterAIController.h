// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "LLL_BossMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_BossMonsterAIController : public ALLL_MonsterBaseAIController
{
	GENERATED_BODY()
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
};