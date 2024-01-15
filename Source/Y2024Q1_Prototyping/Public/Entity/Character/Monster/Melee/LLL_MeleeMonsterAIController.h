// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "LLL_MeleeMonsterAIController.generated.h"

class ULLL_MeleeMonsterDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_MeleeMonsterAIController : public ALLL_MonsterBaseAIController
{
	GENERATED_BODY()
	
public:
	ALLL_MeleeMonsterAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MeleeMonsterDataAsset> MeleeMonsterDataAsset;
};
