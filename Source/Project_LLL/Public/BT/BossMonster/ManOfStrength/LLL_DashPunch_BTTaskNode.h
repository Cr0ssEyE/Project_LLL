﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LLL_DashPunch_BTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_DashPunch_BTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	ULLL_DashPunch_BTTaskNode();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
