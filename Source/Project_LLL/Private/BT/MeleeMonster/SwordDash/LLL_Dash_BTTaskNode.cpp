// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/MeleeMonster/SwordDash/LLL_Dash_BTTaskNode.h"

#include "AIController.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"

ULLL_Dash_BTTaskNode::ULLL_Dash_BTTaskNode()
{
	NodeName = TEXT("Dash (SwordDash)");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_Dash_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const ALLL_SwordDash* SwordDash = Cast<ALLL_SwordDash>(OwnerComp.GetAIOwner()->GetPawn());
	if (!IsValid(SwordDash))
	{
		UE_LOG(LogTemp, Warning, TEXT("대시할 수 있는 몬스터가 아닙니다"));
		
		return EBTNodeResult::Failed;
	}

	SwordDash->Dash();

	return EBTNodeResult::InProgress;
}

void ULLL_Dash_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_SwordDash* SwordDash = Cast<ALLL_SwordDash>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(SwordDash) && !SwordDash->IsDashing())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
