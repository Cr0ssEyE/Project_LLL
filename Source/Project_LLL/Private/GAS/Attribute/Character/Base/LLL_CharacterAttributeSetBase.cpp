// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_CharacterAttributeSetBase::ULLL_CharacterAttributeSetBase() :
	AttackSpeed(100.f),
	ReceiveDamageRate(1.0f)
{
	
}

void ULLL_CharacterAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	
	if (Attribute == GetMoveSpeedAttribute())
	{
		const ACharacter* OwnerCharacter = CastChecked<ACharacter>(GetOwningActor());
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
	}
	
	const ALLL_BaseCharacter* OwnerCharacter = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	OwnerCharacter->UpdateWidgetDelegate.Broadcast();

	if (Attribute != GetReceiveDamageAttribute() && Attribute != GetCurrentHealthAttribute())
	{
		UE_LOG(LogTemp, Log, TEXT("%s의 %s가 변경 %f -> %f"), *GetOwningActor()->GetName(), *Attribute.GetName(), OldValue, NewValue)
	}
}

bool ULLL_CharacterAttributeSetBase::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude < 0.f)
		{
			Data.EvaluatedData.Magnitude = 0.f;
		}
	}
	
	return Super::PreGameplayEffectExecute(Data);
}

void ULLL_CharacterAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		const bool DOT = Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;
		Character->TakeDamageDelegate.Broadcast(DOT);

		//어빌리티에게 피해를 입힌 대상을 전달하는 방법. TryActivate가 아닌 SendGameplayEvent라 Ability Triggers에 태그 할당 필요
		ALLL_BaseCharacter* Attacker = Cast<ALLL_BaseCharacter>(Data.EffectSpec.GetEffectContext().Get()->GetInstigator());
		if (IsValid(Attacker))
		{
			FGameplayEventData PayloadData;
			PayloadData.Instigator = Attacker;
			Attacker->UpdateWidgetDelegate.Broadcast();
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), TAG_GAS_DAMAGED, PayloadData);
		}
		
		SetReceiveDamage(0.f);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));
	}
	
	Character->UpdateWidgetDelegate.Broadcast();

	Super::PostGameplayEffectExecute(Data);
}

