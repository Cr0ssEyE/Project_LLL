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

	const ALLL_BossMonster* BossMonster = CastChecked<ALLL_BossMonster>(OwnerComp.GetAIOwner()->GetPawn());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(BossMonster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	const float MaxHealth = MonsterAttributeSet->GetMaxHealth();
	const float CurrentHealth = MonsterAttributeSet->GetCurrentHealth();
	const float HealthRate = CurrentHealth / MaxHealth * 100.0f;

	uint8 FirstPattern = 0;
	uint8 LastPattern = 0;

	// Todo : 다른 보스 몬스터의 패턴 확장성을 위한 개선 필요
	// Todo : 추후 데이터화 예정
	if (HealthRate <= 100.0f && HealthRate > 70.0f)
	{
		FirstPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_Shockwave);
		LastPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_Dash);
	}
	else if (HealthRate <= 70.0f && HealthRate > 30.0f)
	{
		FirstPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_Shockwave);
		LastPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_SnapOtherMonster);
	}
	else if (HealthRate <= 30.0f)
	{
		FirstPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_Dash);
		LastPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_ApneaInAttack);
	}

	//const uint8 Pattern = FMath::RandRange(FirstPattern, LastPattern);
	const uint8 Pattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_ApneaInAttack);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_PATTERN, Pattern);

	return EBTNodeResult::Succeeded;
}
