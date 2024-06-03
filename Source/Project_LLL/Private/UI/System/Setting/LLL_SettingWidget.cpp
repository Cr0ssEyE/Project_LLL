// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_SettingWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void ULLL_SettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CloseButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::CloseSettingWidget);
	DisplaySettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchDisplaySettingWidget);
	DisplaySettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::HideMainWidget);
	GraphicSettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchGraphicSettingWidget);
	GraphicSettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::HideMainWidget);
	SoundSettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchSoundSettingWidget);
	SoundSettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::HideMainWidget);
	GameplaySettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchGameplaySettingWidget);
	GameplaySettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::HideMainWidget);

	SettingWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
}

void ULLL_SettingWidget::CloseSettingWidget()
{
	if (MainCanvasPanel->GetVisibility() == ESlateVisibility::Hidden)
	{
		ShowMainWidget();
		return;
	}
	
	SettingWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	SwitchDisplaySettingWidget();
	SetRenderScale(FVector2d::Zero());
	SetIsEnabled(false);
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_SettingWidget::HideMainWidget()
{
	SettingWidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
	MainCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	MainCanvasPanel->SetRenderScale(FVector2d::Zero());
}

void ULLL_SettingWidget::ShowMainWidget()
{
	SettingWidgetSwitcher->SetVisibility(ESlateVisibility::Hidden);
	MainCanvasPanel->SetVisibility(ESlateVisibility::Visible);
	MainCanvasPanel->SetRenderScale(FVector2d::One());
}
