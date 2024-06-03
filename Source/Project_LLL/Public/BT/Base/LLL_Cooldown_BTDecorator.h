// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Cooldown.h"
#include "LLL_Cooldown_BTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_Cooldown_BTDecorator : public UBTDecorator_Cooldown
{
	GENERATED_BODY()

public:
	ULLL_Cooldown_BTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
