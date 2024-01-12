// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_PlayerIsInRange_BTDecorator.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"


ULLL_PlayerIsInRange_BTDecorator::ULLL_PlayerIsInRange_BTDecorator()
{
	NodeName = TEXT("Player Is In Range");
}

bool ULLL_PlayerIsInRange_BTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));

	const float Distance = MonsterBase->GetDistanceTo(PlayerBase);

	if (Distance <= MonsterBase->GetAttackDistance())
	{
		return true;
	}

	return false;
}
