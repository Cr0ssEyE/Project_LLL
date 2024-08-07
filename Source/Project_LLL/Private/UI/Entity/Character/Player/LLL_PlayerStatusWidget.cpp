// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerStatusWidget.h"

#include "Components/TextBlock.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"

void ULLL_PlayerStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet)
{
	Super::UpdateWidgetView(CharacterAttributeSet);
	const float MaxHealth = CharacterAttributeSet->GetMaxHealth();
	const float CurrentHealth = CharacterAttributeSet->GetCurrentHealth();

	if(MaxHealth)
	{
		HealthTextBlock->SetText(FText::FromString(FString::FromInt(CurrentHealth).Append(TEXT(" / ")).Append(FString::FromInt(MaxHealth))));
	}
	else
	{
		HealthTextBlock->SetText(FText::FromString(TEXT("!!! MAX HEALTH ERROR !!!")));
	}
}