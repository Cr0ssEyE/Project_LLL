// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_Charge_BTTaskNode.h"

#include "AIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

ULLL_Charge_BTTaskNode::ULLL_Charge_BTTaskNode()
{
	NodeName = TEXT("Charge");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_Charge_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster->CanPlayChargeAnimation())
	{
		Monster->Charge();
	}

	return EBTNodeResult::InProgress;
}

void ULLL_Charge_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster->CanPlayChargeAnimation())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
