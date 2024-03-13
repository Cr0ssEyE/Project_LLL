// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Player/LLL_PlayerStatusWidget.h"

#include "Components/TextBlock.h"
#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"

void ULLL_PlayerStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet) const
{
	Super::UpdateWidgetView(CharacterAttributeSet);

	const float MaxHealth = CharacterAttributeSet->GetMaxHealth();
	const float CurrentHealth = CharacterAttributeSet->GetCurrentHealth();
	const float MaxShield = CharacterAttributeSet->GetMaxShield();
	const float CurrentShield = CharacterAttributeSet->GetCurrentShield();

	if(MaxHealth)
	{
		HealthTextBlock->SetText(FText::FromString(FString::FromInt(CurrentHealth).Append(TEXT(" / ")).Append(FString::FromInt(MaxHealth))));
	}
	else
	{
		HealthTextBlock->SetText(FText::FromString(TEXT("!!! MAX HEALTH ERROR !!!")));
	}
	if(MaxShield)
	{
		ShieldTextBlock->SetText(FText::FromString(FString::FromInt(CurrentShield).Append(TEXT(" / ")).Append(FString::FromInt(MaxShield))));
	}
	else
	{
		ShieldTextBlock->SetText(FText::FromString(TEXT("!!! MAX SHIELD ERROR !!!")));
	}
}
