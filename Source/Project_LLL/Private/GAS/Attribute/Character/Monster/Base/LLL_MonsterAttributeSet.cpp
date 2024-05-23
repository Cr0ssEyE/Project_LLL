// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Monster/Base/LLL_MonsterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

void ULLL_MonsterAttributeSet::ReceiveDamageEvent(const FGameplayEffectModCallbackData& Data)
{
	Super::ReceiveDamageEvent(Data);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());
	if(GetCurrentShield() > 0.f)
	{
		SetCurrentShield(FMath::Clamp(GetCurrentShield() - GetReceiveDamage(), 0.f, GetMaxShield()));
	}
	else
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetReceiveDamage(), 0.f, GetMaxHealth()));
	}
	SetReceiveDamage(0.f);

	if(GetCurrentShield() > 0)
	{
		SetCurrentShield(FMath::Clamp(GetCurrentShield() - GetReceiveDamage(), 0.f, GetMaxShield()));
	}
	else
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetReceiveDamage(), 0.f, GetMaxHealth()));

		if(GetCurrentHealth() == 0)
		{
			Monster->Dead();
		}
		else
		{
			Monster->Damaged();
		}
	}
}
