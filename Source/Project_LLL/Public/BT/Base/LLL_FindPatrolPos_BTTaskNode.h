// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LLL_FindPatrolPos_BTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_FindPatrolPos_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	ULLL_FindPatrolPos_BTTaskNode();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
