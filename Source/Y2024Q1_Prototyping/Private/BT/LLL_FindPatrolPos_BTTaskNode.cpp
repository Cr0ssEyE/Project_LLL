// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_FindPatrolPos_BTTaskNode.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"

ULLL_FindPatrolPos_BTTaskNode::ULLL_FindPatrolPos_BTTaskNode()
{
	NodeName = TEXT("Find Patrol Pos");
}

EBTNodeResult::Type ULLL_FindPatrolPos_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(MonsterBase->GetWorld());
	if (!NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation PatrolPos;
	if (NavSystem->GetRandomPointInNavigableRadius(MonsterBase->GetActorLocation(), 400, PatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROL_POS, PatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
