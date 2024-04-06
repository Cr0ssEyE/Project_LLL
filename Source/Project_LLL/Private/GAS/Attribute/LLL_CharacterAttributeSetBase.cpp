// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"

#include "GameplayEffectExtension.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ULLL_CharacterAttributeSetBase::ULLL_CharacterAttributeSetBase()
{
	
}

void ULLL_CharacterAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if(Attribute == GetMoveSpeedAttribute())
	{
		ACharacter* OwnerCharacter = CastChecked<ACharacter>(GetOwningActor());
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
	}
	const ALLL_BaseCharacter* OwnerCharacter = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	OwnerCharacter->UpdateWidgetDelegate.Broadcast();
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

	const ALLL_BaseCharacter* OwnerCharacter = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	
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

		ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
		if(GetCurrentShield() > 0)
		{
			SetCurrentShield(FMath::Clamp(GetCurrentShield() - GetReceiveDamage(), 0.f, GetMaxShield()));
		}
		else
		{
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetReceiveDamage(), 0.f, GetMaxHealth()));

				if(GetCurrentHealth() == 0)
				{
					Character->Dead();
				}
				else
				{
					Character->Damaged();
				}
		}
		OwnerCharacter->TakeDamageDelegate.Broadcast();
	}
	OwnerCharacter->UpdateWidgetDelegate.Broadcast();
}
