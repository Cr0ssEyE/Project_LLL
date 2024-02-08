// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"

ULLL_PlayerGameplayAbilityBase::ULLL_PlayerGameplayAbilityBase()
{
	
}

void ULLL_PlayerGameplayAbilityBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

}

void ULLL_PlayerGameplayAbilityBase::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	
}
