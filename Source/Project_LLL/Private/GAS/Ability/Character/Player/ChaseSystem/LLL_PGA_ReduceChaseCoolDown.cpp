// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ReduceChaseCoolDown.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ControlChaseHand.h"
#include "GAS/ASC/LLL_BaseASC.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"

void ULLL_PGA_ReduceChaseCoolDown::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerRequiredTag.IsValid() && !TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(CurrentEventData.TargetData, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	ULLL_BaseASC* OwnerASC = Cast<ULLL_BaseASC>(GetAbilitySystemComponentFromActorInfo_Checked());
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles = OwnerASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_PLAYER_STATE_CHASE_COOLDOWN));
	
	if (ActiveGameplayEffectHandles.IsEmpty())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(OwnerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	OwnerASC->ReduceCoolDownEffectDuration(ActiveGameplayEffectHandles[0], PlayerCharacterAttributeSet->GetChaseReduceCoolDownPerHit());
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}