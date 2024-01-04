// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_CheckMonsterHP_BTDecorator.h"

#include "AIController.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"

ULLL_CheckMonsterHP_BTDecorator::ULLL_CheckMonsterHP_BTDecorator()
{
	NodeName = TEXT("Check Monster HP");
}

bool ULLL_CheckMonsterHP_BTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	check(OwnerComp.GetAIOwner())
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (Monster->CheckCharacterHealth() <= 0)
	{
		return true;
	}

	return false;
}
