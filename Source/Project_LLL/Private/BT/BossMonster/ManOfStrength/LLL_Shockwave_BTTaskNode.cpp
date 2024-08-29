// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BossMonster/ManOfStrength/LLL_Shockwave_BTTaskNode.h"

#include "AIController.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"

ULLL_Shockwave_BTTaskNode::ULLL_Shockwave_BTTaskNode()
{
	NodeName = TEXT("Shockwave");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_Shockwave_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ALLL_ManOfStrength* ManOfStrength = Cast<ALLL_ManOfStrength>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ManOfStrength)
	{
		UE_LOG(LogTemp, Warning, TEXT("힘 있는 자가 아닙니다"));
		
		return EBTNodeResult::Failed;
	}

	ManOfStrength->Shockwave();

	return EBTNodeResult::InProgress;
}

void ULLL_Shockwave_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_ManOfStrength* ManOfStrength = Cast<ALLL_ManOfStrength>(OwnerComp.GetAIOwner()->GetPawn());
	if (ManOfStrength && !ManOfStrength->IsAttacking())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}