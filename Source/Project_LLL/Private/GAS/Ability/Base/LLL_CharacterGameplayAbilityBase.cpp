// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Base/LLL_CharacterGameplayAbilityBase.h"

ULLL_CharacterGameplayAbilityBase::ULLL_CharacterGameplayAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_CharacterGameplayAbilityBase::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	
	// UE 5.3 이후로 버그인지 의도된 사항인지 코스트 항목에 GE를 넣어놔도 자동으로 작동하지 않음. 수동으로 호출해야 함.
	if(CurrentActorInfo)
	{
		K2_CommitAbilityCost();
	}
}

void ULLL_CharacterGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// UE 5.3 이후로 버그인지 의도된 사항인지 쿨다운 항목에 GE를 넣어놔도 자동으로 작동하지 않음. 수동으로 호출해야 함.
	if(CurrentActorInfo)
	{
		K2_CommitAbilityCooldown();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_CharacterGameplayAbilityBase::OnCompleteCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_CharacterGameplayAbilityBase::OnInterruptedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}