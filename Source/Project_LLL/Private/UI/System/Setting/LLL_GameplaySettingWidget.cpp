// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_GameplaySettingWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Enumeration/LLL_UserInterfaceEnumHelper.h"
#include "Util/Save/LLL_CustomGameUserSettings.h"

void ULLL_GameplaySettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InputSettingButton->OnClicked.AddDynamic(this, &ULLL_GameplaySettingWidget::OpenInputSettingWidget);

	GameLanguageComboBox->ClearOptions();
	for (auto EnumType : TEnumRange<ELanguageType>())
	{
		GameLanguageComboBox->AddOption(StaticEnum<ELanguageType>()->GetDisplayNameTextByIndex(static_cast<int32>(EnumType)).ToString());
	}
	GameLanguageComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplyGameLanguage);

	SubtitleLanguageComboBox->ClearOptions();
	for (auto EnumType : TEnumRange<ELanguageType>())
	{
		SubtitleLanguageComboBox->AddOption(StaticEnum<ELanguageType>()->GetDisplayNameTextByIndex(static_cast<int32>(EnumType)).ToString());
	}
	SubtitleLanguageComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplySubtitleLanguage);

	SubtitleSizeComboBox->ClearOptions();
	for (auto EnumType : TEnumRange<ESubtitleSize>())
	{
		SubtitleSizeComboBox->AddOption(StaticEnum<ESubtitleSize>()->GetDisplayNameTextByValue(static_cast<int32>(EnumType)).ToString());
	}
	SubtitleSizeComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplySubtitleSize);
	
	SubtitleSpeedComboBox->ClearOptions();
	for (auto EnumType : TEnumRange<ESubtitleSpeed>())
	{
		SubtitleSpeedComboBox->AddOption(StaticEnum<ESubtitleSpeed>()->GetDisplayNameTextByValue(static_cast<int32>(EnumType)).ToString());
	}
	SubtitleSpeedComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ApplySubtitleSpeed);

	CameraShakeActivateCheckBox->OnCheckStateChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ToggleCameraShake);
	TargetingCorrectionActivateCheckBox->OnCheckStateChanged.AddDynamic(this, &ULLL_GameplaySettingWidget::ToggleTargetingCorrection);

	GameLanguageComboBox->SetSelectedOption(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetGameLanguage());
	GameLanguageComboBox->OnSelectionChanged.Broadcast(GameLanguageComboBox->GetSelectedOption(), ESelectInfo::Type::OnMouseClick);

	SubtitleLanguageComboBox->SetSelectedOption(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetSubtitleLanguage());
	SubtitleLanguageComboBox->OnSelectionChanged.Broadcast(SubtitleLanguageComboBox->GetSelectedOption(), ESelectInfo::Type::OnMouseClick);

	const FString SubtitleSizeOption = StaticEnum<ESubtitleSize>()->GetDisplayNameTextByValue(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetSubtitleSize()).ToString();
	SubtitleSizeComboBox->SetSelectedOption(SubtitleSizeOption);
	SubtitleSizeComboBox->OnSelectionChanged.Broadcast(SubtitleSizeComboBox->GetSelectedOption(), ESelectInfo::Type::OnMouseClick);

	const FString SubtitleSpeedOption = StaticEnum<ESubtitleSpeed>()->GetDisplayNameTextByValue(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetSubtitleSpeed()).ToString();
	SubtitleSpeedComboBox->SetSelectedOption(SubtitleSpeedOption);
	SubtitleSpeedComboBox->OnSelectionChanged.Broadcast(SubtitleSpeedComboBox->GetSelectedOption(), ESelectInfo::Type::OnMouseClick);

	CameraShakeActivateCheckBox->SetIsChecked(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->CheckUsingCameraShake());
	CameraShakeActivateCheckBox->OnCheckStateChanged.Broadcast(CameraShakeActivateCheckBox->IsChecked());

	TargetingCorrectionActivateCheckBox->SetIsChecked(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->CheckUsingTargetingCorrection());
	TargetingCorrectionActivateCheckBox->OnCheckStateChanged.Broadcast(TargetingCorrectionActivateCheckBox->IsChecked());
}

void ULLL_GameplaySettingWidget::OpenInputSettingWidget()
{
	
}

void ULLL_GameplaySettingWidget::ApplyGameLanguage(FString SelectionName, ESelectInfo::Type Info)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetGameLanguage(SelectionName);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplyGameplaySettings();
}

void ULLL_GameplaySettingWidget::ApplySubtitleLanguage(FString SelectionName, ESelectInfo::Type Info)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetSubtitleLanguage(SelectionName);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplyGameplaySettings();
}

void ULLL_GameplaySettingWidget::ApplySubtitleSize(FString SelectionName, ESelectInfo::Type Info)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetSubtitleSize(StaticEnum<ESubtitleSize>()->GetValueByNameString(SelectionName));
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplyGameplaySettings();
}

void ULLL_GameplaySettingWidget::ApplySubtitleSpeed(FString SelectionName, ESelectInfo::Type Info)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetSubtitleSpeed(StaticEnum<ESubtitleSpeed>()->GetValueByNameString(SelectionName));
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplyGameplaySettings();
}

void ULLL_GameplaySettingWidget::ToggleCameraShake(bool Value)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetCameraShake(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(true);
}

void ULLL_GameplaySettingWidget::ToggleTargetingCorrection(bool Value)
{
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetTargetingCorrection(Value);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(true);
}
