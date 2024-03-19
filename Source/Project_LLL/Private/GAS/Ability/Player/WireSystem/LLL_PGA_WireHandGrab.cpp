// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandGrab.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_WireHandGrab::ULLL_PGA_WireHandGrab()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	GrabElapsedTime = 0.f;
	MaxGrabDuration = 0.f;
}

bool ULLL_PGA_WireHandGrab::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(IsActive() || !TryGrabAroundEntity(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	return Result;
}

void ULLL_PGA_WireHandGrab::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_WireHandGrab::OnReleasedCallBack);
	
	GrabElapsedTime = 0.f;
	MaxGrabDuration = PlayerWireHand->GetWireHandAttributeSet()->GetGrabDuration();
	GrabTargetEntity();
}

void ULLL_PGA_WireHandGrab::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_WireHandGrab::OnReleasedCallBack);
	PlayerWireHand->K2_DetachFromActor(EDetachmentRule::KeepWorld);
	PlayerWireHand->SetGrabbedActor(nullptr);
	
	if(true) //!bWasCancelled)
	{
		const FGameplayTagContainer ReleaseHandTags(TAG_GAS_WIRE_RELEASE);
		GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(ReleaseHandTags);
	}
	
	GrabElapsedTime = 0.f;
	MaxGrabDuration = 0.f;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool ULLL_PGA_WireHandGrab::TryGrabAroundEntity(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	float SphereRadius = PlayerWireHand->GetCollisionComponent()->GetScaledSphereRadius();

	FHitResult Result;
	FCollisionQueryParams Params;

	GetWorld()->SweepSingleByChannel(
		Result,
		PlayerWireHand->GetActorLocation(),
		PlayerWireHand->GetActorLocation(),
		FQuat::Identity,
		ECC_ENEMY_HIT,
		FCollisionShape::MakeSphere(SphereRadius),
		Params
	);

	if(Result.GetActor())
	{
		PlayerWireHand->SetGrabbedActor(Result.GetActor());
		return true;
	}
	
	return false;
}

void ULLL_PGA_WireHandGrab::GrabTargetEntity()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);

	USphereComponent* WireHandCollision = PlayerWireHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerWireHand->GetHandMesh();
	
	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WireHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerWireHand->AttachToActor(PlayerWireHand->GetGrabbedActor(), FAttachmentTransformRules::KeepRelativeTransform);
	PlayerWireHand->SetActorLocation(PlayerWireHand->GetGrabbedActor()->GetActorLocation());
	
	BP_ApplyGameplayEffectToTarget(UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(PlayerWireHand->GetGrabbedActor()), GrabTargetApplyEffect);
	
	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	WireHandProjectile->Velocity = FVector::Zero();
	WireHandProjectile->Deactivate();
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandGrab::CheckGrabbedTime);
}

void ULLL_PGA_WireHandGrab::CheckGrabbedTime()
{
	if(MaxGrabDuration <= 0.f)
	{
		return;
	}
	
	GrabElapsedTime += GetWorld()->GetDeltaSeconds();
	if(GrabElapsedTime >= MaxGrabDuration)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandGrab::CheckGrabbedTime);
}

void ULLL_PGA_WireHandGrab::OnReleasedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
