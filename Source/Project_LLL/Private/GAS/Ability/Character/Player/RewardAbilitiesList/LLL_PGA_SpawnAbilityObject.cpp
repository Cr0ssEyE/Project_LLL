// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_SpawnAbilityObject.h"

#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"

void ULLL_PGA_SpawnAbilityObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_AbilityObject* AbilityObject = GetWorld()->SpawnActorDeferred<ALLL_AbilityObject>(AbilityObjectClass, GetAvatarActorFromActorInfo()->GetActorTransform());
	AbilityObject->SetAbilityInfo(AbilityData, GetAbilityLevel());
	AbilityObject->FinishSpawning(GetAvatarActorFromActorInfo()->GetActorTransform());
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
