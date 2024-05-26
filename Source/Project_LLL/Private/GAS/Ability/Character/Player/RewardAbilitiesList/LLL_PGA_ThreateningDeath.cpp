// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_ThreateningDeath.h"

#include "Entity/Object/Ability/LLL_Player_ThreateningDeath.h"

void ULLL_PGA_ThreateningDeath::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FRotator Rotation = GetAvatarActorFromActorInfo()->GetActorRotation();
	GetWorld()->SpawnActor(ALLL_Player_ThreateningDeath::StaticClass(), &Location, &Rotation);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
