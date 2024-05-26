// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_PhysicalProof.h"

#include "Entity/Object/Skill/LLL_Player_Deer_PhysicalProof.h"

void ULLL_PGA_PhysicalProof::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FRotator Rotation = GetAvatarActorFromActorInfo()->GetActorRotation();
	GetWorld()->SpawnActor(ALLL_Player_Deer_PhysicalProof::StaticClass(), &Location, &Rotation);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
