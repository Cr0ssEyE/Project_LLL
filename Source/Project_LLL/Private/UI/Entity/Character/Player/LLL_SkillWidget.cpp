// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_SkillWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerSkillAttributeSet.h"

void ULLL_SkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_SkillWidget::UpdateWidgetView(const UAbilitySystemComponent* CharacterASC) const
{
	const ULLL_PlayerSkillAttributeSet* SkillAttributeSet = Cast<ULLL_PlayerSkillAttributeSet>(CharacterASC->GetAttributeSet(ULLL_PlayerSkillAttributeSet::StaticClass()));
	const float MaxSkillGauge = SkillAttributeSet->GetMaxSkillGauge();
	const float CurrentSkillGauge = SkillAttributeSet->GetCurrentSkillGauge();

	if(MaxSkillGauge > 0.f)
	{
		SkillGaugeBar->SetPercent(CurrentSkillGauge / MaxSkillGauge);
		SkillGaugeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), SkillGaugeBar->GetPercent() * 100.f).Append(TEXT("%"))));
	}
}

