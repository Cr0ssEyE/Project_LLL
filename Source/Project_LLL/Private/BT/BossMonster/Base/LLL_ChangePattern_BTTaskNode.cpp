// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BossMonster/Base/LLL_ChangePattern_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"
#include "Enumeration/LLL_BossMonsterPatternEnumHelper.h"

ULLL_ChangePattern_BTTaskNode::ULLL_ChangePattern_BTTaskNode()
{
	NodeName = TEXT("Change Pattern");
}

EBTNodeResult::Type ULLL_ChangePattern_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	uint8 FirstPattern = 0;
	uint8 LastPattern = 0;
	
	if (Cast<ALLL_ManOfStrength>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		FirstPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_Shockwave);
		LastPattern = static_cast<uint8>(EBossMonsterPattern::ManOfStrength_SnapOtherMonster);
	}

	const uint8 Pattern = FMath::RandRange(FirstPattern, LastPattern);
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(BBKEY_PATTERN, Pattern);

	return EBTNodeResult::Succeeded;
}
