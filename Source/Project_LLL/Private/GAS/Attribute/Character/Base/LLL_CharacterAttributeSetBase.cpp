// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "GameFramework/CharacterMovementComponent.h"

ULLL_CharacterAttributeSetBase::ULLL_CharacterAttributeSetBase()
{
	
}

void ULLL_CharacterAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if(Attribute == GetMoveSpeedAttribute())
	{
		const ACharacter* OwnerCharacter = CastChecked<ACharacter>(GetOwningActor());
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
	
	const ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		ReceiveDamageEvent(Data);
		Character->TakeDamageDelegate.Broadcast();

		//05/11 조강건 코드리뷰 중 주석 추가
		//어빌리티에게 피해를 입힌 대상을 전달하는 방법. TryActivate가 아닌 SendGameplayEvent라 Ability Triggers에 태그 할당 필요
		FGameplayEventData PayloadData;
		AActor* Instigator = Data.EffectSpec.GetEffectContext().Get()->GetInstigator();
		if (const ALLL_ThrownObject* ThrownObject = Cast<ALLL_ThrownObject>(Instigator))
		{
			Instigator = ThrownObject->GetOwner();
		}
		PayloadData.Instigator = Instigator;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), TAG_GAS_DAMAGED, PayloadData);
	}
	Character->UpdateWidgetDelegate.Broadcast();
}
