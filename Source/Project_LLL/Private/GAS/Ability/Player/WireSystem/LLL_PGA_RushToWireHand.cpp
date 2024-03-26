// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_RushToWireHand.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

ULLL_PGA_RushToWireHand::ULLL_PGA_RushToWireHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	TargetLocation = Direction = FVector::Zero();
	AbilityEndDistance = 200.f;
	RushSpeed = 0.f;
}

void ULLL_PGA_RushToWireHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));

	PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
	PlayerCharacter->SetActorRotation((PlayerWireHand->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal().Rotation());
	PlayerCharacter->GetCharacterMovement()->Velocity = FVector::Zero();

	PlayerWireHand->GetCollisionComponent()->SetCollisionObjectType(ECC_PLAYER_CHECK);
	PlayerWireHand->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_RushToWireHand::OnReleasedCallBack);

	TargetLocation = PlayerWireHand->GetActorLocation();
	Direction = (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	RushSpeed = PlayerAttributeSet->GetRushSpeed();
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand);
}

void ULLL_PGA_RushToWireHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	PlayerCharacter->GetCharacterMovement()->Velocity = PlayerCharacter->GetCharacterMovement()->Velocity.GetSafeNormal() * PlayerCharacter->GetCharacterMovement()->GetMaxSpeed();
	
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_RushToWireHand::OnReleasedCallBack);

	if(bWasCancelled)
	{
		const FGameplayTagContainer ReleaseHandTags(TAG_GAS_WIRE_RELEASE);
		PlayerWireHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ReleaseHandTags);
	}
	
	TargetLocation = Direction = FVector::Zero();
	RushSpeed = 0.f;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand()
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	float Distance2D = FVector::DistXY(TargetLocation, PlayerCharacter->GetActorLocation());
	
	if(RushSpeed <= 0.f || Distance2D < AbilityEndDistance)
	{
		const FGameplayTagContainer ReleaseHandTags(TAG_GAS_WIRE_RELEASE);
		PlayerWireHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ReleaseHandTags);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	Direction = (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	PlayerCharacter->GetCharacterMovement()->Velocity += Direction * RushSpeed;
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand);
}

void ULLL_PGA_RushToWireHand::OnReleasedCallBack()
{
	// EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
