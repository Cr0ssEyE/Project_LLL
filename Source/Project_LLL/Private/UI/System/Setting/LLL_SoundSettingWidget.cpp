// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_SoundSettingWidget.h"

#include "FMODBlueprintStatics.h"
#include "Components/CheckBox.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Util/Save/LLL_CustomGameUserSettings.h"

void ULLL_SoundSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	MasterSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplyMasterSliderValue);
	BGMSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplyBGMSliderValue);
	SFXSoundVolumeSlider->OnValueChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplySFXSliderValue);
	
	MasterSoundVolumeIgnoreCheckBox->OnCheckStateChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplyMasterVolumeIgnoreState);
	BGMSoundVolumeIgnoreCheckBox->OnCheckStateChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplyBGMVolumeIgnoreState);
	SFXSoundVolumeIgnoreCheckBox->OnCheckStateChanged.AddDynamic(this, &ULLL_SoundSettingWidget::ApplySFXVolumeIgnoreState);

	float MasterVolume = ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetMasterSoundVolume();
	MasterVolume = FMath::Clamp(MasterVolume, 0.f, 1.f);
	MasterSoundVolumeSlider->SetValue(MasterVolume);
	MasterSoundVolumeSlider->OnValueChanged.Broadcast(MasterSoundVolumeSlider->GetValue());

	float BGMVolume = ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetBGMSoundVolume();
	BGMVolume = FMath::Clamp(BGMVolume, 0.f, 1.f);
	BGMSoundVolumeSlider->SetValue(BGMVolume);
	BGMSoundVolumeSlider->OnValueChanged.Broadcast(BGMSoundVolumeSlider->GetValue());

	float SFXVolume = ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetSFXSoundVolume();
	SFXVolume = FMath::Clamp(SFXVolume, 0.f, 1.f);
	SFXSoundVolumeSlider->SetValue(SFXVolume);
	SFXSoundVolumeSlider->OnValueChanged.Broadcast(SFXSoundVolumeSlider->GetValue());

	MasterSoundVolumeIgnoreCheckBox->SetIsChecked(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetMasterSoundIgnored());
	MasterSoundVolumeIgnoreCheckBox->OnCheckStateChanged.Broadcast(MasterSoundVolumeIgnoreCheckBox->IsChecked());

	BGMSoundVolumeIgnoreCheckBox->SetIsChecked(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetBGMSoundIgnored());
	BGMSoundVolumeIgnoreCheckBox->OnCheckStateChanged.Broadcast(BGMSoundVolumeIgnoreCheckBox->IsChecked());

	SFXSoundVolumeIgnoreCheckBox->SetIsChecked(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetSFXSoundIgnored());
	SFXSoundVolumeIgnoreCheckBox->OnCheckStateChanged.Broadcast(SFXSoundVolumeIgnoreCheckBox->IsChecked());
}

void ULLL_SoundSettingWidget::ApplyMasterSliderValue(const float Value)
{
	// MasterSoundClass->Properties.Volume = Value;
	if (!IFMODStudioModule::IsAvailable())
	{
		return;
	}

	MasterVolumeProgressBar->SetPercent(Value);
	FMOD::Studio::System* StudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
	FMOD::Studio::Bus* MasterBus;
	StudioSystem->getBus("bus:/", &MasterBus);
	MasterBus->setVolume(Value);
	
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetMasterSoundVolume(Value);
}

void ULLL_SoundSettingWidget::ApplyBGMSliderValue(const float Value)
{
	BGMVolumeProgressBar->SetPercent(Value);
	UFMODBlueprintStatics::BusSetVolume(AmbienceBus, Value);
	UFMODBlueprintStatics::BusSetVolume(BackgroundBus, Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetBGMSoundVolume(Value);
}

void ULLL_SoundSettingWidget::ApplySFXSliderValue(const float Value)
{
	SFXVolumeProgressBar->SetPercent(Value);
	UFMODBlueprintStatics::BusSetVolume(SoundEffectBus, Value);
	UFMODBlueprintStatics::BusSetVolume(InterfaceBus, Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetSFXSoundVolume(Value);
}

void ULLL_SoundSettingWidget::ApplyMasterVolumeIgnoreState(bool Value)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetMasterSoundIgnored(Value);
	if (!Value)
	{
		ApplyMasterSliderValue(MasterSoundVolumeSlider->GetValue());
		return;
	}
	
	FMOD::Studio::System* StudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
	FMOD::Studio::Bus* MasterBus;
	StudioSystem->getBus("bus:/", &MasterBus);
	MasterBus->setVolume(0.f);
}

void ULLL_SoundSettingWidget::ApplyBGMVolumeIgnoreState(bool Value)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetBGMSoundIgnored(Value);
	if (!Value)
	{
		ApplySFXSliderValue(SFXSoundVolumeSlider->GetValue());
		return;
	}

	UFMODBlueprintStatics::BusSetVolume(AmbienceBus, 0.f);
	UFMODBlueprintStatics::BusSetVolume(BackgroundBus, 0.f);
}

void ULLL_SoundSettingWidget::ApplySFXVolumeIgnoreState(bool Value)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetSFXSoundIgnored(Value);
	if (!Value)
	{
		ApplySFXSliderValue(SFXSoundVolumeSlider->GetValue());
		return;
	}
	
	UFMODBlueprintStatics::BusSetVolume(SoundEffectBus, 0.f);
	UFMODBlueprintStatics::BusSetVolume(InterfaceBus, 0.f);
}
