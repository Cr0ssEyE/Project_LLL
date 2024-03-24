// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/LLL_SkillWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

void ULLL_SkillWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_SkillWidget::UpdateWidgetView(const ULLL_PlayerAttributeSet* CharacterAttributeSet) const
{
	const float MaxSkillGauge = CharacterAttributeSet->GetMaxSkillGauge();
	const float CurrentSkillGauge = CharacterAttributeSet->GetCurrentSkillGauge();

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("스킬 게이지 현재값과 최대값: %f , %f."), CurrentSkillGauge, MaxSkillGauge));
	
	if(MaxSkillGauge > 0.f)
	{
		SkillGaugeBar->SetPercent(CurrentSkillGauge / MaxSkillGauge);
		SkillGaugeText->SetText(FText::FromString(FString::Printf(TEXT("%d"), FMath::FloorToInt(SkillGaugeBar->GetPercent() * 100.f)).Append(TEXT("%"))));
	}
}
