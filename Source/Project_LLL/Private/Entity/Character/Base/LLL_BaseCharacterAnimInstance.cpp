// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_MeshSocketName.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ULLL_BaseCharacterAnimInstance::ULLL_BaseCharacterAnimInstance()
{
	bIsIdle = true;
	bIsFalling = false;
	bIsJumping = false;
	GroundSpeed = 0.0f;
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
	SetStepEventParameter(SOCKET_LEFT_FOOT);
}

void ULLL_BaseCharacterAnimInstance::AnimNotify_RightStep()
{
	SetStepEventParameter(SOCKET_RIGHT_FOOT);
}

void ULLL_BaseCharacterAnimInstance::SetStepEventParameter(FName FootSocketName)
{
	if (!Character->GetMesh()->DoesSocketExist(FootSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("소켓이 존재하지 않음"));
		ensure(false);
		
		return;
	}

	const FVector StartLocation = Character->GetMesh()->GetSocketLocation(FootSocketName);
	const FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -1.0f);
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Character);
	CollisionParams.bReturnPhysicalMaterial = true;
	
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_WorldStatic, CollisionParams))
	{
		return;
	}

	if (HitResult.PhysMaterial == nullptr)
	{
		return;
	}

	SurfaceType = HitResult.PhysMaterial->SurfaceType;
}
