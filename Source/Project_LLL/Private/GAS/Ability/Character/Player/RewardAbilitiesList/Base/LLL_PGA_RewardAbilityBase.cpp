// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"

#include "Constant/LLL_GameplayTags.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"

void ULLL_PGA_RewardAbilityBase::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	
	for (auto ActivateEffect : OnActivateEffects)
	{
		const ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(ActivateEffect.Key.GetDefaultObject());
		if (IsValid(Effect) && ActivateEffect.Value == EEffectApplyTarget::Self)
		{
			const FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(Effect->GetClass(), GetAbilityLevel());
			const float MagnitudeValue = AbilityData->AbilityValue + AbilityData->ChangeValue * (GetAbilityLevel() - 1) / static_cast<uint32>(AbilityData->AbilityValueType);
			EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_CHANGEABLE_VALUE, MagnitudeValue);

 			K2_ApplyGameplayEffectSpecToOwner(EffectHandle);
		}
	}
}

void ULLL_PGA_RewardAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("보상 어빌리티 발동 : %s"), *GetName());
}
