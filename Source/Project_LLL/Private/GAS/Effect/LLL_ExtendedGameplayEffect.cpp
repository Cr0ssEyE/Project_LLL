// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"

#include "GameplayEffectComponents/AbilitiesGameplayEffectComponent.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "GAS/Effect/LLL_GE_GiveAbilityComponent.h"


void ULLL_ExtendedGameplayEffect::SetAbilityInfo(FAbilityDataTable* InAbilityData)
{
	AbilityData = InAbilityData;
}

void ULLL_ExtendedGameplayEffect::SendInfoToAbility()
{
	if (!FindComponent<ULLL_GE_GiveAbilityComponent>())
	{
		return;
	}
	
	ULLL_GE_GiveAbilityComponent* AbilitiesGameplayEffectComponent = &FindOrAddComponent<ULLL_GE_GiveAbilityComponent>();
	
	for (auto& AbilitySpecConfig : AbilitiesGameplayEffectComponent->GetAbilitySpecConfigs())
	{
		if (ULLL_PGA_RewardAbilityBase* RewardAbility = Cast<ULLL_PGA_RewardAbilityBase>(AbilitySpecConfig.Ability.GetDefaultObject()))
		{
			
		}
	}
}
