
#pragma once

#include "CoreMinimal.h"
#include "Constant/LLL_GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
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
			UE_LOG(LogTemp, Log, TEXT("%f Period 값 변경"), EffectSpec->GetPeriod());
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

	static bool SpawnAbilityObject(const ULLL_CharacterGameplayAbilityBase* OwnerAbility, const TSubclassOf<ALLL_AbilityObject>& AbilityObjectClass, FGameplayEventData EventData = FGameplayEventData(), EEffectApplyTarget AbilityObjectLocationTarget = EEffectApplyTarget::Self)
	{
		UWorld* World = OwnerAbility->GetWorld();
		if (!World)
		{
			return false;	
		}
		
		FTransform SpawnTransform = FTransform::Identity;
		if (AbilityObjectLocationTarget == EEffectApplyTarget::Self)
		{
			SpawnTransform = OwnerAbility->GetAvatarActorFromActorInfo()->GetActorTransform();
		}
		else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(EventData.TargetData, 0))
		{
			SpawnTransform = EventData.TargetData.Data[0]->GetActors()[0]->GetActorTransform();
		}
	
		ALLL_AbilityObject* AbilityObject = World->SpawnActorDeferred<ALLL_AbilityObject>(AbilityObjectClass, SpawnTransform);
		if (const ULLL_PGA_RewardAbilityBase* RewardAbility = Cast<ULLL_PGA_RewardAbilityBase>(OwnerAbility))
		{
			AbilityObject->SetAbilityInfo(RewardAbility->GetAbilityData(), RewardAbility->GetAbilityLevel());
		}
		AbilityObject->SetOwner(OwnerAbility->GetAvatarActorFromActorInfo());
		AbilityObject->FinishSpawning(OwnerAbility->GetAvatarActorFromActorInfo()->GetActorTransform());

		if (AbilityObject)
		{
			return true;
		}
		return false;
	}
};
