// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

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

	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(CharacterAttributeSet);
	const float MaxMana = PlayerAttributeSet->GetMaxMana();
	const float CurrentMana = PlayerAttributeSet->GetCurrentMana();
	ChargeGaugeBar->SetPercent(CurrentMana / MaxMana);

	if(MaxMana)
	{
		ManaTextBlock->SetText(FText::FromString(FString::FromInt(CurrentMana).Append(TEXT(" / ")).Append(FString::FromInt(MaxMana))));
	}
	else
	{
		ManaTextBlock->SetText(FText::FromString(TEXT("!!! MAX MANA ERROR !!!")));
	}
}
