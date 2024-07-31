// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_MoveTo_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_MoveTo_BTTaskNode::ULLL_MoveTo_BTTaskNode()
{
	NodeName = TEXT("LLL Move To");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_MoveTo_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const EBTNodeResult::Type NodeResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	FVector TargetLocation;
	if (!GetTargetLocation(OwnerComp, TargetLocation))
	{
		return EBTNodeResult::Failed;
	}
	
	const FVector Direction = TargetLocation - Monster->GetActorLocation();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	if (Monster->GetActorRotation().Equals(Rotation, MonsterAttributeSet->GetFieldOfView() / 2.0f))
	{
		return NodeResult;
	}
	
	return EBTNodeResult::InProgress;
}

void ULLL_MoveTo_BTTaskNode::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	FVector TargetLocation;
	if (!GetTargetLocation(OwnerComp, TargetLocation))
	{
		return;
	}
	
	const FVector Direction = TargetLocation - Monster->GetActorLocation();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	const float TurnSpeed = MonsterAttributeSet->GetTurnSpeed();
	FRotator CalculatedRotation = FMath::RInterpTo(Monster->GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), TurnSpeed);
	CalculatedRotation.Pitch = 0.0f;
	Monster->SetActorRotation(CalculatedRotation);
}

bool ULLL_MoveTo_BTTaskNode::GetTargetLocation(UBehaviorTreeComponent& OwnerComp, FVector& OutTargetLocation) const
{
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		UObject* KeyValue = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
		AActor* TargetActor = Cast<AActor>(KeyValue);
		if (IsValid(TargetActor))
		{
			const ALLL_BaseCharacter* Character = Cast<ALLL_BaseCharacter>(TargetActor);
			if (IsValid(Character) && Character->CheckCharacterIsDead())
			{
				return false;
			}
			OutTargetLocation = TargetActor->GetActorLocation();
			return true;
		}
		return false;
	}
	OutTargetLocation = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	return true;
}
