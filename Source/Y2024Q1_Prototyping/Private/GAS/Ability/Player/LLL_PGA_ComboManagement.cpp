// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_ComboManagement.h"

#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

ULLL_PGA_ComboManagement::ULLL_PGA_ComboManagement()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboStackDuration = MaxComboStackDuration = CurrentComboCount = 0.f;
}

void ULLL_PGA_ComboManagement::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	PlayerAttributes = CastChecked<ULLL_PlayerAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));

	if(IsValid(PlayerAttributes))
	{
		CurrentComboCount = 0;
		MaxComboStackDuration = PlayerAttributes->GetMaxComboStackDuration();
		CurrentComboStackDuration = MaxComboStackDuration;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ComboManagement::ComboTimerTick);
}

void ULLL_PGA_ComboManagement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if(!OnEndedEffects.IsEmpty())
	{
		for (auto EndedEffect : OnEndedEffects)
		{
			if(IsValid(EndedEffect.Key) && EndedEffect.Value == EEffectApplyTarget::Self)
			{
				BP_ApplyGameplayEffectToOwner(EndedEffect.Key);
			}
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ComboManagement::ComboTimerTick()
{
	CurrentComboStackDuration -= GetWorld()->GetDeltaSeconds();
	if(CurrentComboStackDuration)
	{
		if(CurrentComboCount < PlayerAttributes->GetCurrentComboCount())
		{
			CurrentComboCount = PlayerAttributes->GetCurrentComboCount();
			CurrentComboStackDuration = MaxComboStackDuration;
		}
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ComboManagement::ComboTimerTick);
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	// TODO: UI 연결
}
