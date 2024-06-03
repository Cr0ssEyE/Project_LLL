// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"

ULLL_CharacterAttributeSetBase::ULLL_CharacterAttributeSetBase() :
	AttackSpeed(100.f)
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
	const ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		const bool DOT = Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;
		Character->TakeDamageDelegate.Broadcast(DOT);

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

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterHitCheckDebug() && Cast<ALLL_MonsterBase>(GetOwningActor()))
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("몬스터 데미지 입음. : %f"), Data.EvaluatedData.Magnitude));
			}
		}
#endif
		SetReceiveDamage(0.f);
	}
	Character->UpdateWidgetDelegate.Broadcast();

	Super::PostGameplayEffectExecute(Data);
}

