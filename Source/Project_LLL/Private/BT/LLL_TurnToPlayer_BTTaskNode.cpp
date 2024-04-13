// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_TurnToPlayer_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_TurnToPlayer_BTTaskNode::ULLL_TurnToPlayer_BTTaskNode()
{
	NodeName = TEXT("Turn To Player");
}

EBTNodeResult::Type ULLL_TurnToPlayer_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ALLL_MonsterBase* MonsterBase = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	if (IsValid(PlayerBase))
	{
		FVector Direction = PlayerBase->GetActorLocation() - MonsterBase->GetActorLocation();
		Direction.Z = 0.0f;
		const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(MonsterBase->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		const float TurnSpeed = MonsterAttributeSet->GetTurnSpeed();
		MonsterBase->SetActorRotation(FMath::RInterpTo(MonsterBase->GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), TurnSpeed));

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
