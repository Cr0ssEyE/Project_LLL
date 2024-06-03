// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Player/LLL_PlayerSkillAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Util/LLL_MathHelper.h"

ULLL_PlayerSkillAttributeSet::ULLL_PlayerSkillAttributeSet() :
	MaxSkillGauge(100.f),
	SkillGaugeAmplifyByCombo(1.f),
	SkillGaugeAmplifyByItem(1.f)

{
	
}

void ULLL_PlayerSkillAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	if (Data.EvaluatedData.Attribute == GetAddCurrentSkillGaugeAttribute())
	{
		const float Result = FLLL_MathHelper::CalculatePlayerSkillGaugeIncrement(Data.EvaluatedData.Magnitude, GetSkillGaugeAmplifyByCombo(), GetSkillGaugeAmplifyByItem());
		const float NewCurrentSkillGauge = FMath::Clamp(GetCurrentSkillGauge() + Result, 0.f, GetMaxSkillGauge());
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckPlayerSkillDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("스킬 게이지 증감 작동. %f -> %f. 입력값 %f"), GetCurrentSkillGauge(), NewCurrentSkillGauge, GetAddCurrentSkillGauge()));
			}
		}
#endif
		
		SetCurrentSkillGauge(NewCurrentSkillGauge);
		SetAddCurrentSkillGauge(0.f);
	}

	const ALLL_BaseCharacter* OwnerCharacter = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	OwnerCharacter->UpdateWidgetDelegate.Broadcast();
}
