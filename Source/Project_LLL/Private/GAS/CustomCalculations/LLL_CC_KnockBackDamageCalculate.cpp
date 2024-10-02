// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CustomCalculations/LLL_CC_KnockBackDamageCalculate.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

float ULLL_CC_KnockBackDamageCalculate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = Super::CalculateBaseMagnitude_Implementation(Spec);
	
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Spec.GetEffectContext().GetInstigator());
	const ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Spec.GetEffectContext().GetSourceObject());
	if (!IsValid(Player) || !IsValid(Monster))
	{
		return Result;
	}

	const float KnockBackPower = Monster->GetLastKnockBackPower();
	const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	Result = FMath::Floor(FMath::Sqrt(KnockBackPower) + FMath::Sqrt(KnockBackPower + KnockBackPower * PlayerAttributeSet->GetKnockBackConstant()));
	Result *= PlayerAttributeSet->GetAllOffencePowerRate();
	Result *= PlayerAttributeSet->GetKnockBackOffencePowerRate();
	
	// 질량 축적 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_INCREASE_KNOCK_BACK_DAMAGE))
	{
		UE_LOG(LogTemp, Log, TEXT("이누리아 갯수 : %d"), Player->GetEnuriaCount())
		Result *= 1 + Player->GetIncreaseKnockBackDamageByEnuriaCountDamageRate() * Player->GetEnuriaCount();
	}
	Result += PlayerAttributeSet->GetAllOffencePowerPlus();
	Result += PlayerAttributeSet->GetKnockBackOffencePowerPlus();

	UE_LOG(LogTemp, Log, TEXT("넉백 피해 : %f"), Result)
	return Result;
}
