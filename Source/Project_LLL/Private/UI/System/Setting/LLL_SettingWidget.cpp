// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_SettingWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"

void ULLL_SettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CloseButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::CloseSettingWidget);
	DisplaySettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchDisplaySettingWidget);
	GraphicSettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchGraphicSettingWidget);
	SoundSettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchSoundSettingWidget);
	GameplaySettingButton->OnClicked.AddDynamic(this, &ULLL_SettingWidget::SwitchGameplaySettingWidget);
}

void ULLL_SettingWidget::CloseSettingWidget()
{
	SetRenderScale(FVector2d::Zero());
	GEngine->GetGameUserSettings()->ApplySettings(true);
}