// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_MoveToPlayer_BTTask.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_MoveToPlayer_BTTask::ULLL_MoveToPlayer_BTTask()
{
	NodeName = TEXT("Move To Player");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_MoveToPlayer_BTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	const ALLL_MonsterBase* MonsterBase = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(MonsterBase->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	if (!IsValid(PlayerBase))
	{
		return NodeResult;
	}

	const FVector Direction = PlayerBase->GetActorLocation() - MonsterBase->GetActorLocation();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	if (MonsterBase->GetActorRotation().Equals(Rotation, MonsterAttributeSet->GetFieldOfView() / 2.0f))
	{
		return NodeResult;
	}
	
	return EBTNodeResult::InProgress;
}

void ULLL_MoveToPlayer_BTTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ALLL_MonsterBase* MonsterBase = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(MonsterBase->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	
	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	if (!IsValid(PlayerBase))
	{
		return;
	}

	const FVector Direction = PlayerBase->GetActorLocation() - MonsterBase->GetActorLocation();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	const float TurnSpeed = MonsterAttributeSet->GetTurnSpeed();
	MonsterBase->SetActorRotation(FMath::RInterpTo(MonsterBase->GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), TurnSpeed));
}
