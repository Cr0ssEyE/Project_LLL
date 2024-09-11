// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"

ULLL_MonsterAttributeSet::ULLL_MonsterAttributeSet() :
	Weight(1)
{
	
}

bool ULLL_MonsterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	bool Result = Super::PreGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		FGameplayTagContainer TagContainer(TAG_GAS_STATUS_BLEEDING);
		if (GetOwningAbilitySystemComponentChecked()->HasAnyMatchingGameplayTags(TagContainer))
		{
			CheckAbnormalStatus(Data);
		}
	}
	return Result;
}

void ULLL_MonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		AActor* Attacker = Data.EffectSpec.GetEffectContext().Get()->GetInstigator();
		const bool DOT = Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;
		if (const ALLL_ThrownObject* ThrownObject = Cast<ALLL_ThrownObject>(Attacker))
		{
			Attacker = ThrownObject->GetOwner();
		}
		
		ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());
		if (GetCurrentShield() > 0)
		{
			SetCurrentShield(FMath::Clamp(GetCurrentShield() - GetReceiveDamage(), 0.f, GetMaxShield()));
		
			Monster->Damaged(Attacker, DOT);
		}
		else
		{
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetReceiveDamage(), 0.f, GetMaxHealth()));

			if (GetCurrentHealth() == 0)
			{
				Monster->Dead();
			}
			else
			{
				Monster->Damaged(Attacker, DOT);
			}
		}
	}
	
	Super::PostGameplayEffectExecute(Data);
}

void ULLL_MonsterAttributeSet::CheckAbnormalStatus(const FGameplayEffectModCallbackData& Data)
{
	float Damage = Data.EvaluatedData.Magnitude;
	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	if (Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	{
		Data.EvaluatedData.Magnitude = Damage;
		return;
	}

	Data.EvaluatedData.Magnitude = Damage;
}
