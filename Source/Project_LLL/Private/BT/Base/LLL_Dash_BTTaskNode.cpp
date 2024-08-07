// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/MeleeMonster/SwordDash/LLL_Dash_BTTaskNode.h"

#include "AIController.h"
#include "Interface/LLL_DashMonsterInterface.h"

ULLL_Dash_BTTaskNode::ULLL_Dash_BTTaskNode()
{
	NodeName = TEXT("Dash");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_Dash_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const ILLL_DashMonsterInterface* DashMonster = Cast<ILLL_DashMonsterInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (!DashMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("대시할 수 있는 몬스터가 아닙니다"));
		
		return EBTNodeResult::Failed;
	}

	DashMonster->Dash();

	return EBTNodeResult::InProgress;
}

void ULLL_Dash_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const ILLL_DashMonsterInterface* DashMonster = Cast<ILLL_DashMonsterInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (DashMonster && !DashMonster->IsDashing())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
