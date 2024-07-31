// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_FindPatrolPos_BTTaskNode.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_FindPatrolPos_BTTaskNode::ULLL_FindPatrolPos_BTTaskNode()
{
	NodeName = TEXT("Find Patrol Pos");
}

EBTNodeResult::Type ULLL_FindPatrolPos_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Monster->GetWorld());
	if (IsValid(NavSystem))
	{
		FNavLocation PatrolPos;
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		if (NavSystem->GetRandomPointInNavigableRadius(Monster->GetActorLocation(), MonsterAttributeSet->GetFindPatrolPosRadius(), PatrolPos))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROL_POS, PatrolPos.Location);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
