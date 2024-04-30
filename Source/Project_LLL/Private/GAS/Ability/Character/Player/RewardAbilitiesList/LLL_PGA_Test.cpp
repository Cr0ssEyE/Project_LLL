// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_Test.h"

void ULLL_PGA_Test::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UE_LOG(LogTemp, Log, TEXT("어빌리티 : %s, 랭크 : %s"), *GetName(), *StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(AbilityRank)));

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
