// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Monster/Base/LLL_MonsterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "Kismet/GameplayStatics.h"

void ULLL_MonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());
		const bool DOT = Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;
		
		if (GetCurrentShield() > 0)
		{
			SetCurrentShield(FMath::Clamp(GetCurrentShield() - GetReceiveDamage(), 0.f, GetMaxShield()));
		
			Monster->Damaged(DOT);
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
				Monster->Damaged(DOT);
			}
		}
		
		FGameplayTagContainer TagContainer(TAG_GAS_STATUS_MARKED);
		TagContainer.AddTag(TAG_GAS_STATUS_TARGETED);
		TagContainer.AddTag(TAG_GAS_STATUS_BLEEDING);
		if (GetOwningAbilitySystemComponentChecked()->HasAnyMatchingGameplayTags(TagContainer))
		{
			CheckAbnormalStatus(Data);
		}
	}
	
	Super::PostGameplayEffectExecute(Data);
}

void ULLL_MonsterAttributeSet::CheckAbnormalStatus(const FGameplayEffectModCallbackData& Data)
{
	float Damage = GetReceiveDamage();
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));
	if (GetOwningAbilitySystemComponentChecked()->HasMatchingGameplayTag(TAG_GAS_STATUS_MARKED))
	{
		int32 MarkCount = GetOwningAbilitySystemComponentChecked()->GetGameplayTagCount(TAG_GAS_MARK_STACK);
		Damage *= 1.f + AbnormalStatusAttributeSet->GetMarkStatusDamageAmplifyPerStack() * MarkCount;
	}

	if (Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	{
		SetReceiveDamage(Damage);
		return;
	}
	
	if (GetOwningAbilitySystemComponentChecked()->HasMatchingGameplayTag(TAG_GAS_STATUS_TARGETED))
	{
		// TODO: 태그, 인터페이스, 클래스 등등 암튼 뭔가 써서 보스 유무에 따라 배율 변경하기
		Damage *= AbnormalStatusAttributeSet->GetTargetingStatusDamageAmplifyByNormal();
		// Damage *= AbnormalStatusAttributeSet->GetTargetingStatusDamageAmplifyByBoss();
	}
	
	SetReceiveDamage(Damage);
}
