// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"

#include "Constant/LLL_GameplayTags.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"

void ULLL_PGA_RewardAbilityBase::SetAbilityInfo(const FAbilityDataTable* InAbilityData)
{
	AbilityData = InAbilityData;
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("%f, %f, %f"), static_cast<float>(AbilityData->AbilityRank), AbilityData->AbilityValue, AbilityData->ChangeValue));
}

void ULLL_PGA_RewardAbilityBase::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);
	
	for (auto ActivateEffect : OnActivateEffects)
	{
		ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(ActivateEffect.Key.GetDefaultObject());
		if (IsValid(Effect) && ActivateEffect.Value == EEffectApplyTarget::Self)
		{
			FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(Effect->GetClass(), GetAbilityLevel());
			float MagnitudeValue = AbilityData->AbilityValue + AbilityData->ChangeValue * GetAbilityLevel();
			EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_EFFECT_VALUE, MagnitudeValue);

 			K2_ApplyGameplayEffectSpecToOwner(EffectHandle);
		}
	}
}

void ULLL_PGA_RewardAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("보상 어빌리티 발동 : %s, 등급 : %s"), *GetName(), *StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(AbilityData->AbilityRank)));
}
