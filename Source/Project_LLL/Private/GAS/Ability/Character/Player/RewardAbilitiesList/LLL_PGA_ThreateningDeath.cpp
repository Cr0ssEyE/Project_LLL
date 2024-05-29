// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_ThreateningDeath.h"

#include "Entity/Object/Ability/LLL_PlayerThreateningDeath.h"

void ULLL_PGA_ThreateningDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* SpawnActor = GetWorld()->SpawnActorDeferred<ALLL_PlayerThreateningDeath>(ALLL_PlayerThreateningDeath::StaticClass(), GetAvatarActorFromActorInfo()->GetActorTransform());
	ALLL_PlayerThreateningDeath* PlayerThreateningDeath = CastChecked<ALLL_PlayerThreateningDeath>(SpawnActor);
	PlayerThreateningDeath->SetAbilityInfo(AbilityData, GetAbilityLevel());
	SpawnActor->FinishSpawning(GetAvatarActorFromActorInfo()->GetActorTransform());
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
