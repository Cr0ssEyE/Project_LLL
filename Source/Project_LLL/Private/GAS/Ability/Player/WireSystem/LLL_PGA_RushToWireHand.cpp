// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_RushToWireHand.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"

ULLL_PGA_RushToWireHand::ULLL_PGA_RushToWireHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_RushToWireHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwnerLaunchToWireHand();
}

void ULLL_PGA_RushToWireHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand()
{
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	
}
