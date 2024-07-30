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
		FGameplayTagContainer TagContainer(TAG_GAS_STATUS_MARKED);
		TagContainer.AddTag(TAG_GAS_STATUS_TARGETED);
		TagContainer.AddTag(TAG_GAS_STATUS_BLEEDING);
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
		else if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Attacker))
		{
			if (!DOT)
			{
				Player->SetLastSentDamage(GetReceiveDamage());
			}
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
	
	const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));
	if (GetOwningAbilitySystemComponentChecked()->HasMatchingGameplayTag(TAG_GAS_STATUS_MARKED))
	{
		const int32 MarkCount = GetOwningAbilitySystemComponentChecked()->GetGameplayTagCount(TAG_GAS_MARK_STACK);
		Damage *= 1.f + AbnormalStatusAttributeSet->GetMarkStatusDamageAmplifyPerStack() * MarkCount;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("표식 대미지 배율 적용. 카운트: %d | 최종값: %f"), MarkCount, Damage));
	}

	if (Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	{
		Data.EvaluatedData.Magnitude = Damage;
		return;
	}
	
	if (GetOwningAbilitySystemComponentChecked()->HasMatchingGameplayTag(TAG_GAS_STATUS_TARGETED))
	{
		// TODO: 태그, 인터페이스, 클래스 등등 암튼 뭔가 써서 보스 유무에 따라 배율 변경하기
		Damage *= AbnormalStatusAttributeSet->GetTargetingStatusDamageAmplifyByNormal();
		// Damage *= AbnormalStatusAttributeSet->GetTargetingStatusDamageAmplifyByBoss();
		GetOwningAbilitySystemComponentChecked()->RemoveLooseGameplayTag(TAG_GAS_STATUS_TARGETED);
	}

	Data.EvaluatedData.Magnitude = Damage;
}
