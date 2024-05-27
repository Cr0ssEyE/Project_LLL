// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_WaitForCoolDown_BTTask.h"

#include "AIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_WaitForCoolDown_BTTask::ULLL_WaitForCoolDown_BTTask()
{
	NodeName = TEXT("Wait For CoolDown");
}

EBTNodeResult::Type ULLL_WaitForCoolDown_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const UAbilitySystemComponent* ASC = Monster->GetAbilitySystemComponent();
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ASC->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	WaitTime = MonsterAttributeSet->GetAttackCoolDown();
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
