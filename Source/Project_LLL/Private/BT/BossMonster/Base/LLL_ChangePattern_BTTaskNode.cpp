// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BossMonster/Base/LLL_ChangePattern_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"
#include "Enumeration/LLL_BossMonsterPatternEnumHelper.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_ChangePattern_BTTaskNode::ULLL_ChangePattern_BTTaskNode()
{
	NodeName = TEXT("Change Pattern");
}

EBTNodeResult::Type ULLL_ChangePattern_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ALLL_BossMonster* BossMonster = CastChecked<ALLL_BossMonster>(OwnerComp.GetAIOwner()->GetPawn());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(BossMonster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	const float MaxHealth = MonsterAttributeSet->GetMaxHealth();
	const float CurrentHealth = MonsterAttributeSet->GetCurrentHealth();
	const float HealthRate = CurrentHealth / MaxHealth * 100.0f;

	const ULLL_BossMonsterDataAsset* BossMonsterDataAsset = CastChecked<ULLL_BossMonsterDataAsset>(BossMonster->GetCharacterDataAsset());
	TArray<EBossMonsterPattern> CurrentHavePatterns = BossMonster->GetCurrentHavePatterns();

	for (auto AddPattern : BossMonsterDataAsset->AddPatterns)
	{
		if (HealthRate <= AddPattern.Key && !CurrentHavePatterns.Contains(AddPattern.Value))
		{
			CurrentHavePatterns.Emplace(AddPattern.Value);
		}
	}
	
	for (auto RemovePattern : BossMonsterDataAsset->RemovePatterns)
	{
		if (HealthRate <= RemovePattern.Key && CurrentHavePatterns.Contains(RemovePattern.Value))
		{
			CurrentHavePatterns.Remove(RemovePattern.Value);
		}
	}

	BossMonster->SetCurrentHavePatterns(CurrentHavePatterns);

	if (CurrentHavePatterns.Num() > 0)
	{
		uint8 Pattern;
		
		do
		{
			Pattern = static_cast<uint8>(CurrentHavePatterns[FMath::RandRange(0, CurrentHavePatterns.Num() - 1)]);
		}
		while (CurrentHavePatterns.Num() != 1 && Pattern == OwnerComp.GetBlackboardComponent()->GetValueAsEnum(BBKEY_PATTERN));
		
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_PATTERN, Pattern);
		BossMonster->SetChargeMontageKey(static_cast<EBossMonsterPattern>(Pattern));
		return EBTNodeResult::Succeeded;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("패턴이 존재하지 않습니다"))
	return EBTNodeResult::Failed;
}
