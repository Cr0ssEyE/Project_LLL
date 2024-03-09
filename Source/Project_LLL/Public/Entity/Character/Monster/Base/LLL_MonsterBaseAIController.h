// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LLL_MonsterBaseAIController.generated.h"

class ULLL_MonsterBaseDataAsset;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MonsterBaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterDataAsset;
};
