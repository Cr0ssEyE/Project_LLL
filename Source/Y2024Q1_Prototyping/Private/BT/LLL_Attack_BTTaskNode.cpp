// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_Attack_BTTaskNode.h"

#include "AIController.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"

ULLL_Attack_BTTaskNode::ULLL_Attack_BTTaskNode()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_Attack_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	MonsterBase->Attack();

	return EBTNodeResult::InProgress;
}

void ULLL_Attack_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!MonsterBase->AttackAnimationIsPlaying())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
