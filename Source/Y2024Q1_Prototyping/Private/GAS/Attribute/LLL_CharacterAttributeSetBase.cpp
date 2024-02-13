// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"

ULLL_CharacterAttributeSetBase::ULLL_CharacterAttributeSetBase()
{
	
}

bool ULLL_CharacterAttributeSetBase::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if(Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		if(Data.EvaluatedData.Magnitude < 0.f)
		{
			Data.EvaluatedData.Magnitude = 0.f;
		}
	}
	return Super::PreGameplayEffectExecute(Data);
}

void ULLL_CharacterAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
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

			ALLL_BaseCharacter* Character = Cast<ALLL_BaseCharacter>(GetOwningActor());
			if (IsValid(Character))
			{
				if(GetCurrentHealth() == 0)
				{
					Character->Dead();
				}
				else
				{
					Character->Damaged();
				}
			}
		}
	}
}
