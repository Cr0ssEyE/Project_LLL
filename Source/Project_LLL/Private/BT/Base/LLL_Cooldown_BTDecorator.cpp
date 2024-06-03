// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_Cooldown_BTDecorator.h"

#include "AIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_Cooldown_BTDecorator::ULLL_Cooldown_BTDecorator()
{
	NodeName = TEXT("LLL Cooldown");
}

bool ULLL_Cooldown_BTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	const FBTCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FBTCooldownDecoratorMemory>(NodeMemory);
	const double RecalculateTime = OwnerComp.GetWorld()->GetTimeSeconds() - MonsterAttributeSet->GetAttackCoolDown();
	
	return RecalculateTime >= DecoratorMemory->LastUseTimestamp;
}
