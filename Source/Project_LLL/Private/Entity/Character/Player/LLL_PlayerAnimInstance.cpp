// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"

#include "FMODAudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"

void ULLL_PlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerDataAsset = Cast<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
}

void ULLL_PlayerAnimInstance::AnimNotify_Step()
{
	Super::AnimNotify_Step();
	
	FVector StartLocation = Character->GetActorLocation();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * -2.0f);
	
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
