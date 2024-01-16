// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_CheckRange_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

ULLL_CheckRange_BTService::ULLL_CheckRange_BTService()
{
	NodeName = TEXT("Check Range");
	Interval = 0.3f;
	RandomDeviation = 0.1f;
}

void ULLL_CheckRange_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	if (IsValid(MonsterBase) && IsValid(PlayerBase))
	{
		const float Distance = MonsterBase->GetDistanceTo(PlayerBase);

		if (!PlayerBase->CheckCharacterIsDead() && Distance <= MonsterBase->GetAttackDistance())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_RANGE, true);
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_RANGE, false);
}
