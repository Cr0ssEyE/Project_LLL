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

	MasterSoundVolumeSlider->SetValue(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetMasterSoundVolume());
	MasterSoundVolumeSlider->OnValueChanged.Broadcast(MasterSoundVolumeSlider->GetValue());

	BGMSoundVolumeSlider->SetValue(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetBGMSoundVolume());
	BGMSoundVolumeSlider->OnValueChanged.Broadcast(BGMSoundVolumeSlider->GetValue());

	SFXSoundVolumeSlider->SetValue(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetSFXSoundVolume());
	SFXSoundVolumeSlider->OnValueChanged.Broadcast(SFXSoundVolumeSlider->GetValue());
}

void ULLL_SoundSettingWidget::ApplyMasterSliderValue(const float Value)
{
	// MasterSoundClass->Properties.Volume = Value;
	MasterVolumeProgressBar->SetPercent(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetMasterSoundVolume(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySoundSettings();
}

void ULLL_SoundSettingWidget::ApplyBGMSliderValue(const float Value)
{
	// BGMSoundClass->Properties.Volume = Value;
	BGMVolumeProgressBar->SetPercent(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetBGMSoundVolume(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySoundSettings();
}

void ULLL_SoundSettingWidget::ApplySFXSliderValue(const float Value)
{
	// SFXSoundClass->Properties.Volume = Value;
	SFXVolumeProgressBar->SetPercent(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetSFXSoundVolume(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySoundSettings();
}
