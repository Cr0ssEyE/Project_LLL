// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Object/Interactive/Reward/LLL_RGA_GiveAbility.h"

#include "AbilitySystemComponent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

void ULLL_RGA_GiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();

	const FGameplayAbilitySpec AbilitySpec(CastChecked<UGameplayAbility>(RewardAbility));
	ASC->GiveAbility(AbilitySpec);
}
