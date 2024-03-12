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
	Direction = FVector::Zero();
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
	
	PlayerWireHand->GetCollisionComponent()->SetCollisionObjectType(ECC_PLAYER_CHECK);
	PlayerWireHand->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_RushToWireHand::OnReleasedCallBack);
	
	Direction = (PlayerWireHand->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
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
		GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(ReleaseHandTags);
	}
	
	Direction = FVector::Zero();
	RushSpeed = 0.f;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand()
{
	if(RushSpeed <= 0.f)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	
	Direction = (PlayerWireHand->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	PlayerCharacter->GetCharacterMovement()->Velocity += Direction * RushSpeed;
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand);
}

void ULLL_PGA_RushToWireHand::OnReleasedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
