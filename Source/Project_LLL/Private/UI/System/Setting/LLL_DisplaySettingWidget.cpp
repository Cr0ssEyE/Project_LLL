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
	FResolutionValueHelper ResolutionValueHelper;
	UEnum* ResolutionEnum = FindObject<UEnum>(nullptr, *ResolutionValueHelper.EnumPath, true);
	for (auto ResolutionType : TEnumRange<EResolutionTypes>())
	{
		FString ResolutionString = ResolutionEnum->GetNameStringByValue(static_cast<int64>(ResolutionType));
		ResolutionString.Append(TEXT("("));
		ResolutionString.Append(ResolutionValueHelper.ResolutionValues[static_cast<uint8>(ResolutionType)].Key);
		ResolutionString.Append(TEXT("×"));	
		ResolutionString.Append(ResolutionValueHelper.ResolutionValues[static_cast<uint8>(ResolutionType)].Value);
		ResolutionString.Append(TEXT(")"));	
		
		ResolutionComboBox->AddOption(ResolutionString);
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

	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetFrameRateLimit(60.f);
	ULLL_CustomGameUserSettings::GetCustomGameUserSettings()->SetVSyncEnabled(true);
}

void ULLL_DisplaySettingWidget::ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info)
{
	if(ResolutionName.IsEmpty())
	{
		return;
	}
	
	FResolutionValueHelper ResolutionValueHelper;
	uint32 ScreenWidthValue = FCString::Atoi(*ResolutionValueHelper.ResolutionValues[ResolutionComboBox->GetSelectedIndex()].Key);
	uint32 ScreenHeightValue = FCString::Atoi(*ResolutionValueHelper.ResolutionValues[ResolutionComboBox->GetSelectedIndex()].Value);
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