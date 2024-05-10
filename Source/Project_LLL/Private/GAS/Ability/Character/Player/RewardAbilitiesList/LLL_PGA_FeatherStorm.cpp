// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_FeatherStorm.h"

#include "Entity/Object/Skill/PlayerFeatherStorm/LLL_PlayerFeatherStorm.h"

void ULLL_PGA_FeatherStorm::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("깃털 폭풍 발동"));
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FRotator Rotation = GetAvatarActorFromActorInfo()->GetActorRotation();
	GetWorld()->SpawnActor(ALLL_PlayerFeatherStorm::StaticClass(), &Location, &Rotation);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
