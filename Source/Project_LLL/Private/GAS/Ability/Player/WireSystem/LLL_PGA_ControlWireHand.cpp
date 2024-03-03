// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_ControlWireHand.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_ControlWireHand::ULLL_PGA_ControlWireHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsAlreadyThrown = false;
	bIsReleaseOnGoing = false;
}

void ULLL_PGA_ControlWireHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(PlayerCharacter->GetWireHand());
	
	UAbilityTask_WaitGameplayEvent* WireHandReachedTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, ReachedEventGameplayTag, nullptr, true, true);
	WireHandReachedTask->EventReceived.AddDynamic(this, &ULLL_PGA_ControlWireHand::OnReachedCallBack);
	WireHandReachedTask->ReadyForActivation();
	
	ThrowHand(ActorInfo);
}

void ULLL_PGA_ControlWireHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	bIsAlreadyThrown = false;
	bIsReleaseOnGoing = false;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ControlWireHand::ThrowHand(const FGameplayAbilityActorInfo* ActorInfo)
{
	if(bIsAlreadyThrown)
	{
		return;
	}
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(PlayerCharacter->GetWireHand());

	UAbilitySystemComponent* HandASC = PlayerWireHand->GetAbilitySystemComponent();
	if(IsValid(HandASC))
	{
		// TODO: ThrowHand Ability 및 태그 생성 후 부착 
		const FGameplayTagContainer ThrowHandTags;
		HandASC->TryActivateAbilitiesByTag(ThrowHandTags);
	}
	bIsAlreadyThrown = true;
}

void ULLL_PGA_ControlWireHand::ReleaseHand(const FGameplayAbilityActorInfo* ActorInfo)
{
	if(bIsReleaseOnGoing)
	{
		return;
	}
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(PlayerCharacter->GetWireHand());

	UAbilitySystemComponent* HandASC = PlayerWireHand->GetAbilitySystemComponent();
	if(IsValid(HandASC))
	{
		// TODO: ReleaseHand Ability 및 태그 생성 후 부착
		const FGameplayTagContainer ReleaseHandTags;
		HandASC->TryActivateAbilitiesByTag(ReleaseHandTags);
	}
	bIsReleaseOnGoing = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_ControlWireHand::OnReachedCallBack(FGameplayEventData Payload)
{
	ReleaseHand(CurrentActorInfo);
}
