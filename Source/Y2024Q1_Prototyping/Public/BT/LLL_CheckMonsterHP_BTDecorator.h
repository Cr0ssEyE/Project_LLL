// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "LLL_CheckMonsterHP_BTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_CheckMonsterHP_BTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	ULLL_CheckMonsterHP_BTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
