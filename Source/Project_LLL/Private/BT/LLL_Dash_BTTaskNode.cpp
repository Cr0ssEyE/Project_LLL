// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_Dash_BTTaskNode.h"

ULLL_Dash_BTTaskNode::ULLL_Dash_BTTaskNode()
{
	NodeName = TEXT("Dash");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_Dash_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}

void ULLL_Dash_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
