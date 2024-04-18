// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"

#include "FMODAudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_MeshSocketName.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"

void ULLL_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerDataAsset = Cast<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
}

void ULLL_PlayerAnimInstance::AnimNotify_LeftStep()
{
	Super::AnimNotify_LeftStep();
	
	SetStepEventParameter(SOCKET_LEFT_FOOT);
}

void ULLL_PlayerAnimInstance::AnimNotify_RightStep()
{
	Super::AnimNotify_RightStep();

	SetStepEventParameter(SOCKET_RIGHT_FOOT);
}

void ULLL_PlayerAnimInstance::PlayDeadAnimation()
{
	StopAllMontages(0.0f);
	Montage_Play(PlayerDataAsset->DeadAnimMontage);
}

void ULLL_PlayerAnimInstance::SetStepEventParameter(FName FootSocketName) const
{
	if (!Character->GetMesh()->DoesSocketExist(FootSocketName))
	{
		UE_LOG(LogTemp, Warning, TEXT("소켓이 존재하지 않음"));
		
		return;
	}
	
	FVector StartLocation = Character->GetMesh()->GetSocketLocation(FootSocketName);
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -1.0f);
	
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
	
	for (auto StepEventParameterProperty : PlayerDataAsset->StepEventParameterProperties)
	{
		if (HitResult.PhysMaterial->SurfaceType == StepEventParameterProperty.Key)
		{
			Character->GetFModAudioComponent()->SetParameter(PlayerDataAsset->StepEventParameterName, static_cast<float>(StepEventParameterProperty.Value));
		}
	}
}
