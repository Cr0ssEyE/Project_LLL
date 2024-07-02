// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/RangedMonster/LLL_FindRetreatPos_BTTaskNode.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_FindRetreatPos_BTTaskNode::ULLL_FindRetreatPos_BTTaskNode()
{
	NodeName = TEXT("Find Retreat Pos");
}

EBTNodeResult::Type ULLL_FindRetreatPos_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Monster->GetWorld());
	if (IsValid(NavSystem))
	{
		FNavLocation RetreatPos;
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		if (NavSystem->GetRandomPointInNavigableRadius(Monster->GetActorLocation(), MonsterAttributeSet->GetAttackDistance(), RetreatPos))
		{
			const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
			if (!IsValid(Player))
			{
				return EBTNodeResult::Failed;
			}

			if (FVector::Distance(Player->GetActorLocation(), RetreatPos.Location) < MonsterAttributeSet->GetAttackDistance())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RETREAT_POS_IS_VALID, false);
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_RETREAT_POS_IS_VALID, true);
			}

			OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_RETREAT_POS, RetreatPos.Location);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}
