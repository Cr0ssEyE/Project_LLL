// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_GameplaySettingWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"

void ULLL_GameplaySettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InputSettingButton->OnClicked.AddDynamic(this, &ULLL_GameplaySettingWidget::OpenInputSettingWidget);
	
	GameLanguageComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplyGameLanguage);
	SubtitleLanguageComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplySubtitleLanguage);
	SubtitleOpacityComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplySubtitleOpacity);
	SubtitleSpeedComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplySubtitleSpeed);

	CameraShakeActivateCheckBox->OnCheckStateChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ToggleCameraShake);
	TargetingCorrectionActivateCheckBox->OnCheckStateChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ToggleTargetingCorrection);
}

void ULLL_GameplaySettingWidget::OpenInputSettingWidget()
{
	
}

void ULLL_GameplaySettingWidget::ApplyGameLanguage(FString ResolutionName, ESelectInfo::Type Info)
{
	
}

void ULLL_GameplaySettingWidget::ApplySubtitleLanguage(FString ResolutionName, ESelectInfo::Type Info)
{
	
}

void ULLL_GameplaySettingWidget::ApplySubtitleOpacity(FString ResolutionName, ESelectInfo::Type Info)
{
	
}

void ULLL_GameplaySettingWidget::ApplySubtitleSpeed(FString ResolutionName, ESelectInfo::Type Info)
{
	
}

void ULLL_GameplaySettingWidget::ToggleCameraShake(bool Value)
{
	
}

void ULLL_GameplaySettingWidget::ToggleTargetingCorrection(bool Value)
{
	
}
