// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CustomCalculations/LLL_CC_AttackDamageCalculate.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Util/LLL_AbilityDataHelper.h"
#include "Util/LLL_MathHelper.h"

float ULLL_CC_AttackDamageCalculate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = Super::CalculateBaseMagnitude_Implementation(Spec);
	
	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Spec.GetEffectContext().GetInstigator());
	if (!IsValid(Player))
	{
		return Result;
	}

	const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	// 과충전 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_CHARGE_ATTACK) || Player->CheckChargeTriggered())
	{
		const float OffsetOffencePower = PlayerAttributeSet->GetMaxChargeAttackDamage() - PlayerAttributeSet->GetMinChargeAttackDamage();
		const float TempOffencePower = PlayerAttributeSet->GetMinChargeAttackDamage() + Player->GetChargeAttackChargeRate() * OffsetOffencePower;
		UE_LOG(LogTemp, Log, TEXT("과충전 이누리아로 공격력 %f로 적용"), TempOffencePower)
		Result = TempOffencePower;
	}
	else
	{
		Result = PlayerAttributeSet->GetOffencePower();
	}

	Result = FLLL_MathHelper::CalculateCriticalDamage(Result, Player);
	Result = FLLL_AbilityDataHelper::CalculateOffencePower(Result, Player);
	
	// 과격한 돌진 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_DASH_ATTACK) && Player->CheckDashAttackCanAttack())
	{
		Result += Player->GetDashAttackOffencePowerPlus();
		
		Player->SetDashAttackCanAttack(false);
	}
	
	return Result;
}
