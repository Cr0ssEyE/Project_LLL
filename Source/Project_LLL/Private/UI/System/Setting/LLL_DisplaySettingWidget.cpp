// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_DisplaySettingWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/System/Setting/LLL_SettingWidget.h"
#include "Util/Save/LLL_CustomGameUserSettings.h"

void ULLL_DisplaySettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResolutionComboBox->ClearOptions();
	for (auto ResolutionType : TEnumRange<EResolutionTypes>())
	{
		ResolutionComboBox->AddOption(StaticEnum<EResolutionTypes>()->GetDisplayNameTextByIndex(static_cast<int32>(ResolutionType)).ToString());
	}
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &ULLL_DisplaySettingWidget::ApplyResolutionType);
	
	UseFullScreenButton->OnClicked.AddDynamic(this, &ULLL_DisplaySettingWidget::ApplyFullScreen);
	UseWindowButton->OnClicked.AddDynamic(this, &ULLL_DisplaySettingWidget::ApplyWindowScreen);

	ResolutionComboBox->SetSelectedOption(ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetDisplayResolutionString());
	ResolutionComboBox->OnSelectionChanged.Broadcast(ResolutionComboBox->GetSelectedOption(),  ESelectInfo::Type::OnMouseClick);

	if (ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->GetFullscreenMode() == EWindowMode::WindowedFullscreen)
	{
		UseFullScreenButton->OnClicked.Broadcast();
	}
	else
	{
		UseWindowButton->OnClicked.Broadcast();
	}
}

void ULLL_DisplaySettingWidget::ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info)
{
	if(ResolutionName.IsEmpty())
	{
		return;
	}

	TArray<FString> ScreenResolutionString;
	ResolutionName.ParseIntoArray(ScreenResolutionString, TEXT("×"));
	uint32 ScreenWidthValue = FCString::Atoi(*ScreenResolutionString[0]);
	uint32 ScreenHeightValue = FCString::Atoi(*ScreenResolutionString[1]);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SaveScreenResolutionString(ResolutionName);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetScreenResolution(FIntPoint(ScreenWidthValue, ScreenHeightValue));
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(false);
}

void ULLL_DisplaySettingWidget::ApplyFullScreen()
{
	UseFullScreenButton->SetIsEnabled(false);
	UseWindowButton->SetIsEnabled(true);
	
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(true);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ConfirmVideoMode();
}

void ULLL_DisplaySettingWidget::ApplyWindowScreen()
{
	UseFullScreenButton->SetIsEnabled(true);
	UseWindowButton->SetIsEnabled(false);
	
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ApplySettings(true);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->ConfirmVideoMode();
}