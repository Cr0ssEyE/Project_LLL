// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandRelease.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"

ULLL_PGA_WireHandRelease::ULLL_PGA_WireHandRelease()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_WireHandRelease::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ReleaseToOwnerLocation();
}

void ULLL_PGA_WireHandRelease::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_WireHandRelease::ReleaseToOwnerLocation()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	
	PlayerWireHand->SetReleaseState(PlayerCharacter->GetActorLocation());
}
