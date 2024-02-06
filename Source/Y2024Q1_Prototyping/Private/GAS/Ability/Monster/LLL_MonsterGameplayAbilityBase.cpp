// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/LLL_MonsterGameplayAbilityBase.h"

void ULLL_MonsterGameplayAbilityBase::OnCompleteCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_MonsterGameplayAbilityBase::OnInterruptedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
