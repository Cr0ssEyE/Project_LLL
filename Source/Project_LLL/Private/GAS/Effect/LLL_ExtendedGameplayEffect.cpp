// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"

#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"

void ULLL_ExtendedGameplayEffect::SendRankToAbility()
{
	CastChecked<ULLL_PGA_RewardAbilityBase>(GrantedAbilities[0].Ability)->SetAbilityRank(AbilityRank);
}
