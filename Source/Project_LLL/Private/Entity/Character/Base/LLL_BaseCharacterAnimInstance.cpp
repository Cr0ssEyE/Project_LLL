// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"

#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ULLL_BaseCharacterAnimInstance::ULLL_BaseCharacterAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void ULLL_BaseCharacterAnimInstance::PlayDeadAnimation()
{
	StopAllMontages(0.0f);
	Montage_Play(CharacterDataAsset->DeadAnimMontage);
}

void ULLL_BaseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ALLL_BaseCharacter>(TryGetPawnOwner());
	if (IsValid(Character))
	{
		Movement = Character->GetCharacterMovement();
	}
}

void ULLL_BaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(Character))
	{
		return;
	}

	Velocity = Movement->Velocity;
	GroundSpeed = Velocity.Size2D();
	bIsIdle = GroundSpeed < MovingThreshold;
	bIsFalling = Movement->IsFalling();
	bIsJumping = bIsFalling && (Velocity.Z > JumpingThreshold);
}
