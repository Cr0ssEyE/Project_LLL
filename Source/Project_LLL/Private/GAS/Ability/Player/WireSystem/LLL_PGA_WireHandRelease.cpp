// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandRelease.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_WireHandRelease::ULLL_PGA_WireHandRelease()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_WireHandRelease::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->GetHandMesh()->SetAnimation(ReleaseAnim);

	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_WireHandRelease::OnReleaseCompleteCallBack);
	
	ReleaseToOwnerLocation();
}

void ULLL_PGA_WireHandRelease::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_WireHandRelease::OnReleaseCompleteCallBack);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_WireHandRelease::ReleaseToOwnerLocation()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());

	const FVector WorldLocation = PlayerWireHand->GetActorLocation();
	PlayerWireHand->K2_DetachFromActor();
	PlayerWireHand->SetActorLocation(WorldLocation);
	PlayerWireHand->GetHandMesh()->SetAnimation(ReleaseAnim);
	
	USphereComponent* WireHandCollision = PlayerWireHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerWireHand->GetHandMesh();
	
	WireHandCollision->SetCollisionObjectType(ECC_PLAYER_ONLY);
	WireHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	
	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = PlayerWireHand->GetWireHandAttributeSet();
	WireHandProjectile->Activate();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandRelease::RetargetReleaseVelocity);
}

void ULLL_PGA_WireHandRelease::RetargetReleaseVelocity()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	
	const FVector ToOwnerDirection = (PlayerCharacter->GetActorLocation() - PlayerWireHand->GetActorLocation()).GetSafeNormal();
	PlayerWireHand->SetActorRotation(ToOwnerDirection.Rotation());

	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = PlayerWireHand->GetWireHandAttributeSet();
	
	WireHandProjectile->Velocity = ToOwnerDirection * WireHandAttributeSet->GetReleaseSpeed();
	if(PlayerWireHand->GetCollisionComponent()->GetCollisionObjectType() == ECC_PLAYER_ONLY)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandRelease::RetargetReleaseVelocity);
	}
}

void ULLL_PGA_WireHandRelease::OnReleaseCompleteCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
