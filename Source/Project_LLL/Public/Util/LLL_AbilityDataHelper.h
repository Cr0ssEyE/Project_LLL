
#pragma once

#include "CoreMinimal.h"
#include "Constant/LLL_GameplayTags.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"

class PROJECT_LLL_API FLLL_AbilityDataHelper
{
public:
	// 이펙트의 상태이상 설정 관련,
	static void SetAbnormalStatusAbilityDuration(ULLL_PGA_RewardAbilityBase* RewardAbility, TSharedPtr<FGameplayEffectSpec> EffectSpec)
	{
		const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(RewardAbility->GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));

		// EffectSpec->bDurationLocked = false 하는 이유는 코드 외적인 부분에서 
		if (RewardAbility->AbilityTags.HasTag(TAG_GAS_BLEEDING))
		{
			EffectSpec->bDurationLocked = false;
			EffectSpec->SetDuration(AbnormalStatusAttributeSet->GetBleedingStatusDuration(), true);
			EffectSpec->Period = AbnormalStatusAttributeSet->GetBleedingStatusPeriod();
			return;
		}

		if (RewardAbility->AbilityTags.HasTag(TAG_GAS_MARK))
		{
			EffectSpec->bDurationLocked = false;
			EffectSpec->SetDuration(AbnormalStatusAttributeSet->GetMarkStatusDuration(), true);
			return;
		}

		if (RewardAbility->AbilityTags.HasTag(TAG_GAS_TARGETING))
		{
			EffectSpec->bDurationLocked = false;
			EffectSpec->SetDuration(AbnormalStatusAttributeSet->GetTargetingStatusDuration(), true);
			return;
		}
	}
};
