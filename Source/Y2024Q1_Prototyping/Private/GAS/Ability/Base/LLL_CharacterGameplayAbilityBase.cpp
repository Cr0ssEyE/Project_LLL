// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Base/LLL_CharacterGameplayAbilityBase.h"

void ULLL_CharacterGameplayAbilityBase::OnCompleteCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_CharacterGameplayAbilityBase::OnInterruptedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}