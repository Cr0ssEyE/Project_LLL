// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLL_ExecuteCueHelper.h"

ULLL_BaseCharacterAnimInstance::ULLL_BaseCharacterAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void ULLL_BaseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ALLL_BaseCharacter>(TryGetPawnOwner());
	if (IsValid(Character))
	{
		Movement = Character->GetCharacterMovement();
		CharacterDataAsset = Character->GetCharacterDataAsset();
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

void ULLL_BaseCharacterAnimInstance::AnimNotify_LeftStep()
{
	FLLL_ExecuteCueHelper::ExecuteCue(this, Character->GetAbilitySystemComponent(), CharacterDataAsset->StepCueTag);
}

void ULLL_BaseCharacterAnimInstance::AnimNotify_RightStep()
{
	FLLL_ExecuteCueHelper::ExecuteCue(this, Character->GetAbilitySystemComponent(), CharacterDataAsset->StepCueTag);
}
