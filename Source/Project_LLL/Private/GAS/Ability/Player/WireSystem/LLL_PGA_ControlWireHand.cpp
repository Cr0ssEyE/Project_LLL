// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_ControlWireHand.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_ControlWireHand::ULLL_PGA_ControlWireHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool ULLL_PGA_ControlWireHand::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	
	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	if (!IsValid(PlayerBase))
	{
		return false;
	}
	
	const ALLL_PlayerWireHand* PlayerWireHand = Cast<ALLL_PlayerWireHand>(PlayerBase->GetWireHand());
	if (!IsValid(PlayerWireHand))
	{
		return false;
	}
	return Result;
}

void ULLL_PGA_ControlWireHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(PlayerCharacter->GetWireHand());
	
}

void ULLL_PGA_ControlWireHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ControlWireHand::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
}

void ULLL_PGA_ControlWireHand::ThrowHand(const FGameplayAbilityActorInfo* ActorInfo)
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(PlayerCharacter->GetWireHand());
	ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = PlayerWireHand->GetWireHandAttributeSet();
	
	UProjectileMovementComponent* HandProjectile = PlayerWireHand->GetProjectileComponent();
	HandProjectile->Activate();
	HandProjectile->Velocity = PlayerCharacter->GetActorForwardVector() * WireHandAttributeSet->GetThrowSpeed();
}

void ULLL_PGA_ControlWireHand::ReleaseHand(const FGameplayAbilityActorInfo* ActorInfo)
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(PlayerCharacter->GetWireHand());

	
}
