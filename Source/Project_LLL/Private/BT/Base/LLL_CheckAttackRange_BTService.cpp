// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_CheckAttackRange_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_CheckAttackRange_BTService::ULLL_CheckAttackRange_BTService()
{
	NodeName = TEXT("Check Attack Range");
	Interval = 0.3f;
	RandomDeviation = 0.1f;
}

void ULLL_CheckAttackRange_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	if (IsValid(Player))
	{
		const float Distance = Monster->GetDistanceTo(Player);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				const FColor DebugColor = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW) ? FColor::Green : FColor::Yellow;
				DrawDebugLine(GetWorld(), Monster->GetActorLocation(), Player->GetActorLocation(), DebugColor, false, DeltaSeconds);
			}
		}
#endif

		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		if (Distance <= MonsterAttributeSet->GetAttackDistance())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_RANGE, true);
			return;
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_RANGE, false);
}
