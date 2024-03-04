// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_RushToWireHand.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

ULLL_PGA_RushToWireHand::ULLL_PGA_RushToWireHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_RushToWireHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_RushToWireHand::OnReleasedCallBack);
	
	OwnerLaunchToWireHand();
}

void ULLL_PGA_RushToWireHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	
	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_RushToWireHand::OnReleasedCallBack);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand()
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));

	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
	PlayerWireHand->GetCollisionComponent()->SetCollisionObjectType(ECC_PLAYER_ONLY);
	
	const FVector RushDirection = (PlayerWireHand->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	PlayerCharacter->LaunchCharacter(RushDirection * PlayerAttributeSet->GetRushSpeed() * 1000.f, true, true);
}

void ULLL_PGA_RushToWireHand::OnReleasedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
