// Fill out your copyright notice in the Description page of Project Settings.


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
	if(!bWasCancelled)
	{
		// 단, 도달한 위치 주변에 몬스터가 있다면 그랩으로 전환
		const FGameplayTagContainer GrabTag(TAG_GAS_WIRE_GRAB);
		if(!GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(GrabTag))
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
	
	TargetLocation = PlayerCharacter->GetMouseLocation();
	TargetLocation.Z = PlayerCharacter->GetActorLocation().Z;
	const FVector ThrowDirection = (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();

	PlayerWireHand->SetActorLocationAndRotation(PlayerCharacter->GetActorLocation(), ThrowDirection.Rotation());
	PlayerWireHand->SetActorRotation(ThrowDirection.Rotation());
	
	USphereComponent* WireHandCollision = PlayerWireHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerWireHand->GetHandMesh();
	
	WireHandCollision->SetCollisionObjectType(ECC_ENEMY_HIT);
	WireHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	HandMesh->SetAnimation(ThrowAnim);

	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = PlayerWireHand->GetWireHandAttributeSet();
	WireHandProjectile->Activate();
	WireHandProjectile->Velocity = ThrowDirection * WireHandAttributeSet->GetThrowSpeed();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandThrow::CheckReached);
}

void ULLL_PGA_WireHandThrow::CheckReached()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = PlayerWireHand->GetWireHandAttributeSet();
	
	float LocationDistance = FVector::Distance(PlayerWireHand->GetActorLocation(), TargetLocation);
	if(LocationDistance <= WireHandAttributeSet->GetCorrectionReachStateDistance())
	{
		PlayerWireHand->SetActorLocation(TargetLocation);
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
