// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_ChangeActionEffects.h"

#include "AnimNotify_PlayNiagaraEffect.h"
#include "NiagaraSystem.h"

ULLL_PGA_ChangeActionEffects::ULLL_PGA_ChangeActionEffects() :
	bIsGenerated(false)
{
	
}

bool ULLL_PGA_ChangeActionEffects::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	if (bIsGenerated || !TargetAnimMontage)
	{
		return false;
	}
	
	return Result;
}

void ULLL_PGA_ChangeActionEffects::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	for (auto Notify : TargetAnimMontage->Notifies)
	{
		UAnimNotify_PlayNiagaraEffect* NiagaraEffectNotify = Cast<UAnimNotify_PlayNiagaraEffect>(Notify.Notify);
		if (!NiagaraEffectNotify)
		{
			continue;
		}

		for (int i = 0; i < TargetNiagaraSystem.Num(); ++i)
		{
			if (NiagaraEffectNotify->Template == TargetNiagaraSystem[i])
			{
				NiagaraEffectNotify->Template = NewNiagaraSystem[i];
				// GC 컬렉팅 방지.
				if (!NewNiagaraSystem[i]->IsRooted())
				{
					NewNiagaraSystem[i]->AddToRoot();
				}
			}
		}
	}
	bIsGenerated = true;
}

void ULLL_PGA_ChangeActionEffects::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	for (auto Notify : TargetAnimMontage->Notifies)
	{
		UAnimNotify_PlayNiagaraEffect* NiagaraEffectNotify = Cast<UAnimNotify_PlayNiagaraEffect>(Notify.Notify);
		if (!NiagaraEffectNotify)
		{
			continue;
		}
		
		for (int i = 0; i < TargetNiagaraSystem.Num(); ++i)
		{
			if (NiagaraEffectNotify->Template == NewNiagaraSystem[i])
			{
				NiagaraEffectNotify->Template = TargetNiagaraSystem[i];
				if (!TargetNiagaraSystem[i]->IsRooted())
				{
					TargetNiagaraSystem[i]->AddToRoot();
				}
				NewNiagaraSystem[i]->RemoveFromRoot();
			}
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
