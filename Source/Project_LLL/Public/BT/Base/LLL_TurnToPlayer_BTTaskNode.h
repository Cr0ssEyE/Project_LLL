// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LLL_TurnToPlayer_BTTaskNode.generated.h"

#define ROTATION_RATE_YAW_DEVIDE_NUM 36.0f

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_TurnToPlayer_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	ULLL_TurnToPlayer_BTTaskNode();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
