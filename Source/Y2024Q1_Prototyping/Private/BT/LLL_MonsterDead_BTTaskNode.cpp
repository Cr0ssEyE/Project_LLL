// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_MonsterDead_BTTaskNode.h"

#include "AIController.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"

ULLL_MonsterDead_BTTaskNode::ULLL_MonsterDead_BTTaskNode()
{
	NodeName = TEXT("Monster Dead");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_MonsterDead_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	check(OwnerComp.GetAIOwner());
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	check(Monster);
	Monster->Dead();
	return EBTNodeResult::InProgress;
}

void ULLL_MonsterDead_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	check(OwnerComp.GetAIOwner());
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	check(Monster);
	if (Monster->DeadAnimationIsEnd())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


