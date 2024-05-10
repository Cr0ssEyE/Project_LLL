// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_GameSettingWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Enumeration/UserInterfaceEnumHelper.h"
#include "GameFramework/GameUserSettings.h"

void ULLL_GameSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MasterSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_GameSettingWidget::ApplyMasterSliderValue);
	BGMSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_GameSettingWidget::ApplyBGMSliderValue);
	SFXSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_GameSettingWidget::ApplySFXSliderValue);

	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameSettingWidget::ApplyResolutionType);
	UseFullScreenBtn->OnClicked.AddDynamic(this, &ULLL_GameSettingWidget::ApplyFullScreen);
	UseWindowBtn->OnClicked.AddDynamic(this, &ULLL_GameSettingWidget::ApplyWindowScreen);
	CloseBtn->OnClicked.AddDynamic(this, &ULLL_GameSettingWidget::CloseSettingWidget);
}

void ULLL_GameSettingWidget::ApplyMasterSliderValue(const float Value)
{
	// MasterSoundClass->Properties.Volume = Value;
	MasterVolumeProgressBar->SetPercent(Value);
}

void ULLL_GameSettingWidget::ApplyBGMSliderValue(const float Value)
{
	// BGMSoundClass->Properties.Volume = Value;
	BGMVolumeProgressBar->SetPercent(Value);
}

void ULLL_GameSettingWidget::ApplySFXSliderValue(const float Value)
{
	// SFXSoundClass->Properties.Volume = Value;
	SFXVolumeProgressBar->SetPercent(Value);
}

void ULLL_GameSettingWidget::ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info)
{
	if(ResolutionName.IsEmpty())
	{
		return;
	}

	FString ScreenWidthValueString;
	FString ScreenHeightValueString;
	UEnum::GetDisplayValueAsText(static_cast<EResolutionTypes>(ResolutionComboBox->GetSelectedIndex())).ToString().Split(TEXT("x"), &ScreenWidthValueString, &ScreenHeightValueString);

	uint32 ScreenWidthValue = FCString::Atoi(*ScreenWidthValueString);
	uint32 ScreenHeightValue = FCString::Atoi(*ScreenHeightValueString);
	GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(ScreenWidthValue, ScreenHeightValue));
	ResolutionTypes = static_cast<EResolutionTypes>(ResolutionComboBox->GetSelectedIndex());
	GEngine->GetGameUserSettings()->ApplySettings(false);
}

void ULLL_GameSettingWidget::ApplyFullScreen()
{
	bIsFullScreenActivate = true;
	UseFullScreenBtn->SetIsEnabled(false);
	UseWindowBtn->SetIsEnabled(true);
	GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	GEngine->GetGameUserSettings()->ApplySettings(false);
}

void ULLL_GameSettingWidget::ApplyWindowScreen()
{
	bIsFullScreenActivate = false;
	UseFullScreenBtn->SetIsEnabled(true);
	UseWindowBtn->SetIsEnabled(false);
	GEngine->GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
	GEngine->GetGameUserSettings()->ApplySettings(false);
}
