// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_TurnToPlayer_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ULLL_TurnToPlayer_BTTaskNode::ULLL_TurnToPlayer_BTTaskNode()
{
	NodeName = TEXT("Turn To Player");
}

EBTNodeResult::Type ULLL_TurnToPlayer_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	if (!IsValid(Player))
	{
		return EBTNodeResult::Failed;
	}

	const FVector Direction = Player->GetActorLocation() - Monster->GetActorLocation();
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	const float TurnSpeed = Monster->GetCharacterMovement()->RotationRate.Yaw / ROTATION_RATE_YAW_DEVIDE_NUM;
	FRotator CalculatedRotation = FMath::RInterpTo(Monster->GetActorRotation(), Rotation, GetWorld()->GetDeltaSeconds(), TurnSpeed);
	CalculatedRotation.Pitch = 0.0f;
	Monster->SetActorRotation(CalculatedRotation);

	return EBTNodeResult::Succeeded;
}
