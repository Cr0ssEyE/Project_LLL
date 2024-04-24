// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Object/Base/LLL_ObjectGameplayAbilityBase.h"

ULLL_ObjectGameplayAbilityBase::ULLL_ObjectGameplayAbilityBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_ObjectGameplayAbilityBase::OnCompleteCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_ObjectGameplayAbilityBase::OnInterruptedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
