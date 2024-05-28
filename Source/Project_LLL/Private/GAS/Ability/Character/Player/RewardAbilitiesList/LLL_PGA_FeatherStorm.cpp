// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_FeatherStorm.h"

#include "Entity/Object/Ability/LLL_PlayerFeatherStorm.h"

void ULLL_PGA_FeatherStorm::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* SpawnActor = GetWorld()->SpawnActorDeferred<ALLL_PlayerFeatherStorm>(ALLL_PlayerFeatherStorm::StaticClass(), GetAvatarActorFromActorInfo()->GetActorTransform());
	ALLL_PlayerFeatherStorm* PlayerThreateningDeath = CastChecked<ALLL_PlayerFeatherStorm>(SpawnActor);
	PlayerThreateningDeath->SetAbilityInfo(AbilityData, GetAbilityLevel());
	SpawnActor->FinishSpawning(GetAvatarActorFromActorInfo()->GetActorTransform());
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
