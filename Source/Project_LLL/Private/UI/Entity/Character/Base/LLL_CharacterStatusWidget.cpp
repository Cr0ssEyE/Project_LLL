// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Base/LLL_CharacterStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Constant/LLL_MaterialParameterName.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"

void ULLL_CharacterStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_CharacterStatusWidget::UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet)
{
	if (!IsValid(HealthBarMaterial))
	{
		ensure(false);
		return;
	}
	
	if (!IsValid(HealthBarDynamicMaterial))
	{
		HealthBarDynamicMaterial = UMaterialInstanceDynamic::Create(HealthBarMaterial, this);
		const FProgressBarStyle BaseWidgetStyle = HealthGaugeBar->GetWidgetStyle();
		FProgressBarStyle NewWidgetStyle;
		NewWidgetStyle.SetFillImage(BaseWidgetStyle.FillImage);
		NewWidgetStyle.SetBackgroundImage(BaseWidgetStyle.BackgroundImage);
		NewWidgetStyle.SetMarqueeImage(BaseWidgetStyle.MarqueeImage);
		NewWidgetStyle.SetEnableFillAnimation(BaseWidgetStyle.EnableFillAnimation);
		NewWidgetStyle.FillImage.SetResourceObject(HealthBarDynamicMaterial);

		CurrentHealthBarPercent = CurrentHealthBarPercent = 1.f;
		HealthGaugeBar->SetWidgetStyle(NewWidgetStyle);
		HealthBarDynamicMaterial->SetScalarParameterValue(UI_PROGRESS_TOP, 1.f);
		HealthBarDynamicMaterial->SetScalarParameterValue(UI_PROGRESS_BOTTOM, 1.f);
	}
	
	const float MaxHealth = CharacterAttributeSet->GetMaxHealth();
	const float CurrentHealth = CharacterAttributeSet->GetCurrentHealth();
	float UpdateHealthValue = CurrentHealth / MaxHealth;
	if (GetWorld()->GetTimerManager().IsTimerActive(MaterialAnimTimerHandle) && CurrentHealthBarPercent != UpdateHealthValue)
	{
		HealthGaugeBar->SetPercent(CurrentHealthBarPercent);
		MaterialAnimTimerHandle.Invalidate();
	}

	if (CurrentHealthBarPercent != UpdateHealthValue)
	{
		CurrentHealthBarPercent = UpdateHealthValue;
		HealthBarDynamicMaterial->SetScalarParameterValue(UI_PROGRESS_TOP, CurrentHealthBarPercent);
		CurrentSemiHealthBarPercent = 1.f - CurrentHealthBarPercent;
		GetWorld()->GetTimerManager().SetTimer(MaterialAnimTimerHandle, this, &ULLL_CharacterStatusWidget::UpdateFillMaterial, 0.01f, true);
	}
}

void ULLL_CharacterStatusWidget::UpdateFillMaterial()
{
	CurrentSemiHealthBarPercent -= HealthDecrementBarSpeed;
	if (CurrentSemiHealthBarPercent <= 0.f)
	{
		CurrentSemiHealthBarPercent = 0.f;
		HealthBarDynamicMaterial->SetScalarParameterValue(UI_PROGRESS_BOTTOM, CurrentSemiHealthBarPercent);
		HealthGaugeBar->SetPercent(CurrentHealthBarPercent);
		MaterialAnimTimerHandle.Invalidate();
		return;
		
	}
	HealthBarDynamicMaterial->SetScalarParameterValue(UI_PROGRESS_BOTTOM, CurrentSemiHealthBarPercent);
}
