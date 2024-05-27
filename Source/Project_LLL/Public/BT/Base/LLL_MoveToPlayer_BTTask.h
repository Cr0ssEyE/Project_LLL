// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "LLL_MoveToPlayer_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MoveToPlayer_BTTask : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	ULLL_MoveToPlayer_BTTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
