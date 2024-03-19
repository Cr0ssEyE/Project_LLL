﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandThrow.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_WireHandThrow::ULLL_PGA_WireHandThrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	TargetLocation = FVector::Zero();
}

void ULLL_PGA_WireHandThrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());

	PlayerWireHand->OnGrabbedDelegate.AddDynamic(this, &ULLL_PGA_WireHandThrow::OnGrabbedCallBack);
	PlayerCharacter->PlayerRotateToMouseCursor();
	
	ThrowToCursorLocation();
}

void ULLL_PGA_WireHandThrow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->OnGrabbedDelegate.RemoveDynamic(this, &ULLL_PGA_WireHandThrow::OnGrabbedCallBack);
	
	// Cancel되지 않고 종료한 경우 = 마우스 커서 위치에 도달해 회수 상태로 전환한 경우
	if (!bWasCancelled)
	{
		// 단, 도달한 위치 주변에 몬스터가 있다면 그랩으로 전환
		const FGameplayTagContainer GrabTag(TAG_GAS_WIRE_GRAB);
		if (!GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(GrabTag))
		{
			const FGameplayTagContainer ReleaseTag(TAG_GAS_WIRE_RELEASE);
			GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(ReleaseTag);
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_WireHandThrow::ThrowToCursorLocation()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = PlayerWireHand->GetWireHandAttributeSet();
	
	TargetLocation = PlayerCharacter->GetMouseLocation();
	float TargetDistance = FVector::DistXY(TargetLocation, PlayerCharacter->GetActorLocation());
	// 마우스 위치가 투척 최소거리 보다 가까운 거리일 경우 보정
	if (TargetDistance < WireHandAttributeSet->GetMinimumThrowDistance())
	{
		TargetLocation *= WireHandAttributeSet->GetMinimumThrowDistance() / TargetDistance;
	}
	TargetLocation.Z = PlayerCharacter->GetActorLocation().Z;
	DrawDebugPoint(GetWorld(), TargetLocation, 10.f, FColor::Blue, false, 3.f);
	
	const FVector ThrowDirection = PlayerCharacter->GetActorForwardVector(); // (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();

	PlayerWireHand->SetActorLocationAndRotation(PlayerCharacter->GetActorLocation(), ThrowDirection.Rotation());
	PlayerWireHand->SetActorRotation(ThrowDirection.Rotation());
	
	USphereComponent* WireHandCollision = PlayerWireHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerWireHand->GetHandMesh();
	
	WireHandCollision->SetCollisionObjectType(ECC_ENEMY_HIT);
	WireHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	HandMesh->SetAnimation(ThrowAnim);

	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	WireHandProjectile->Activate();
	WireHandProjectile->Velocity = ThrowDirection * WireHandAttributeSet->GetThrowSpeed();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandThrow::CheckReached);
}

void ULLL_PGA_WireHandThrow::CheckReached()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = PlayerWireHand->GetWireHandAttributeSet();
	
	float LocationDistance = FVector::Distance(PlayerWireHand->GetActorLocation(), TargetLocation);
	float OwnerDistance = FVector::DistXY(PlayerWireHand->GetActorLocation(), PlayerCharacter->GetActorLocation());
	
	if (LocationDistance <= WireHandAttributeSet->GetCorrectionReachStateDistance())
	{
		PlayerWireHand->SetActorLocation(TargetLocation);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else if (OwnerDistance >= WireHandAttributeSet->GetMaximumThrowDistance())
	{
		const FGameplayTagContainer ReleaseTag(TAG_GAS_WIRE_RELEASE);
		GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(ReleaseTag);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandThrow::CheckReached);
	}
}

void ULLL_PGA_WireHandThrow::OnGrabbedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
