// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/LLL_SkillWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

void ULLL_SkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_SkillWidget::UpdateWidgetView(const ULLL_PlayerCharacterAttributeSet* CharacterAttributeSet) const
{
	const float MaxSkillGauge = CharacterAttributeSet->GetMaxSkillGauge();
	const float CurrentSkillGauge = CharacterAttributeSet->GetCurrentSkillGauge();

	if(MaxSkillGauge > 0.f)
	{
		SkillGaugeBar->SetPercent(CurrentSkillGauge / MaxSkillGauge);
		SkillGaugeText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), SkillGaugeBar->GetPercent() * 100.f).Append(TEXT("%"))));
	}
}
