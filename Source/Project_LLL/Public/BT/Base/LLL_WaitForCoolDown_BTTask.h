// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "LLL_WaitForCoolDown_BTTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_WaitForCoolDown_BTTask : public UBTTask_Wait
{
	GENERATED_BODY()

public:
	ULLL_WaitForCoolDown_BTTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
