// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/Save/LLL_CustomGameUserSettings.h"

#include "Enumeration/LLL_UserInterfaceEnumHelper.h"

ULLL_CustomGameUserSettings::ULLL_CustomGameUserSettings(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	MasterSoundVolume(100),
	BGMSoundVolume(100),
	SFXSoundVolume(100),
	bMasterSoundIgnore(false),
	bBGMSoundIgnore(false),
	bSFXSoundIgnore(false),
	bUseCameraShake(true),
	bUseTargetingCorrection(true),
	SubtitleSize(static_cast<uint8>(ESubtitleSize::Medium)),
	SubtitleSpeed(static_cast<uint8>(ESubtitleSpeed::Normal))
{
}

ULLL_CustomGameUserSettings* ULLL_CustomGameUserSettings::GetCustomGameUserSettings()
{
	return CastChecked<ULLL_CustomGameUserSettings>(GEngine->GetGameUserSettings());
}

void ULLL_CustomGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	ApplySoundSettings();
	ApplyGameplaySettings();
	
	Super::ApplySettings(bCheckForCommandLineOverrides);
}

void ULLL_CustomGameUserSettings::ApplySoundSettings()
{
	
}

void ULLL_CustomGameUserSettings::ApplyGameplaySettings()
{
	
}

void ULLL_CustomGameUserSettings::SetToDefaults()
{
	
	Super::SetToDefaults();
}
