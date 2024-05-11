// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_SoundSettingWidget.h"

#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Util/Save/LLL_CustomGameUserSettings.h"

void ULLL_SoundSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	MasterSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplyMasterSliderValue);
	BGMSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplyBGMSliderValue);
	SFXSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplySFXSliderValue);
}

void ULLL_SoundSettingWidget::ApplyMasterSliderValue(const float Value)
{
	// MasterSoundClass->Properties.Volume = Value;
	MasterVolumeProgressBar->SetPercent(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetMasterSoundVolume(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(true);
}

void ULLL_SoundSettingWidget::ApplyBGMSliderValue(const float Value)
{
	// BGMSoundClass->Properties.Volume = Value;
	BGMVolumeProgressBar->SetPercent(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetBGMSoundVolume(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(true);
}

void ULLL_SoundSettingWidget::ApplySFXSliderValue(const float Value)
{
	// SFXSoundClass->Properties.Volume = Value;
	SFXVolumeProgressBar->SetPercent(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetSFXSoundVolume(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(true);
}
