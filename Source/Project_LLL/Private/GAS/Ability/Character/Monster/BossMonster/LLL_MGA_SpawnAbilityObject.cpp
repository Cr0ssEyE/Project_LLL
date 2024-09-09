// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/LLL_MGA_SpawnAbilityObject.h"

#include "Util/LLL_AbilityDataHelper.h"

void ULLL_MGA_SpawnAbilityObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	FLLL_AbilityDataHelper::SpawnAbilityObject(this, AbilityObjectClass, CurrentEventData, AbilityObjectLocationTarget, OffsetLocation);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
