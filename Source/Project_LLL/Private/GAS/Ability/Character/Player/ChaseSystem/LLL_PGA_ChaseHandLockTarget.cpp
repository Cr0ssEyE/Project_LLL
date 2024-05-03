// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ChaseHandLockTarget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHandAttributeSet.h"

ULLL_PGA_ChaseHandLockTarget::ULLL_PGA_ChaseHandLockTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	GrabElapsedTime = 0.f;
	MaxGrabDuration = 0.f;
}

bool ULLL_PGA_ChaseHandLockTarget::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	if(IsActive() || !TryGrabAroundEntity(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	
	return Result;
}

void ULLL_PGA_ChaseHandLockTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	PlayerChaseHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_ChaseHandLockTarget::OnCompleteCallBack);
	
	GrabElapsedTime = 0.f;

	const ULLL_PlayerChaseHandAttributeSet* ChaseHandAttributeSet = CastChecked<ULLL_PlayerChaseHandAttributeSet>(PlayerChaseHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerChaseHandAttributeSet::StaticClass()));
	
	MaxGrabDuration = ChaseHandAttributeSet->GetGrabDuration();
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandLockTarget::GrabTargetEntity);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 그랩 동작")));
		}
	}
#endif
}

void ULLL_PGA_ChaseHandLockTarget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	PlayerChaseHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_ChaseHandLockTarget::OnCompleteCallBack);
	PlayerChaseHand->K2_DetachFromActor(EDetachmentRule::KeepWorld);
	PlayerChaseHand->SetGrabbedActor(nullptr);
	
	// const FGameplayTagContainer ReleaseHandTags(TAG_GAS_WIRE_RELEASE);
	// GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(ReleaseHandTags);
	
	GrabElapsedTime = 0.f;
	MaxGrabDuration = 0.f;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool ULLL_PGA_ChaseHandLockTarget::TryGrabAroundEntity(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	float SphereRadius = PlayerChaseHand->GetCollisionComponent()->GetScaledSphereRadius();

	FHitResult Result;
	FCollisionQueryParams Params;

	GetWorld()->SweepSingleByChannel(
		Result,
		PlayerChaseHand->GetActorLocation(),
		PlayerChaseHand->GetActorLocation(),
		FQuat::Identity,
		ECC_ENEMY_HIT,
		FCollisionShape::MakeSphere(SphereRadius),
		Params
	);

	if(Result.GetActor())
	{
		PlayerChaseHand->SetGrabbedActor(Result.GetActor());
		return true;
	}
	
	return false;
}

void ULLL_PGA_ChaseHandLockTarget::GrabTargetEntity()
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	USphereComponent* HandCollision = PlayerChaseHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerChaseHand->GetHandMesh();
	
	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerChaseHand->AttachToActor(PlayerChaseHand->GetGrabbedActor(), FAttachmentTransformRules::KeepRelativeTransform);
	PlayerChaseHand->SetActorLocation(PlayerChaseHand->GetGrabbedActor()->GetActorLocation());
	
	BP_ApplyGameplayEffectToTarget(UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(PlayerChaseHand->GetGrabbedActor()), GrabTargetApplyEffect);
	
	UProjectileMovementComponent* HandProjectile = PlayerChaseHand->GetProjectileMovementComponent();
	HandProjectile->Velocity = FVector::Zero();
	HandProjectile->Deactivate();

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(PlayerChaseHand->GetOwner());
	Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_PLAYER_CHASE_RUSH));
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandLockTarget::CheckGrabbedTime);
}

void ULLL_PGA_ChaseHandLockTarget::CheckGrabbedTime()
{
	if(MaxGrabDuration <= 0.f)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	PlayerChaseHand->SetActorLocation(PlayerChaseHand->GetGrabbedActor()->GetActorLocation());
	GrabElapsedTime += GetWorld()->GetDeltaSeconds();
	if(GrabElapsedTime >= MaxGrabDuration)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerChaseActionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 그랩 유지시간 종료")));
			}
		}
#endif
	}
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandLockTarget::CheckGrabbedTime);
}

