// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/Setting/LLL_GraphicSettingWidget.h"

#include "Components/Button.h"
#include "Enumeration/LLL_UserInterfaceEnumHelper.h"
#include "GameFramework/GameUserSettings.h"

void ULLL_GraphicSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TextureQualityLowButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyTextureQualityLow);
	TextureQualityMiddleButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyTextureQualityMiddle);
	TextureQualityHighButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyTextureQualityHigh);

	TextureOptionButtons.Emplace(TextureQualityLowButton);
	TextureOptionButtons.Emplace(TextureQualityMiddleButton);
	TextureOptionButtons.Emplace(TextureQualityHighButton);
	
	ShadowQualityLowButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyShadowQualityLow);
	ShadowQualityMiddleButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyShadowQualityMiddle);
	ShadowQualityHighButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyShadowQualityHigh);

	ShadowOptionButtons.Emplace(ShadowQualityLowButton);
	ShadowOptionButtons.Emplace(ShadowQualityMiddleButton);
	ShadowOptionButtons.Emplace(ShadowQualityHighButton);
	
	GIQualityLowButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyGIQualityLow);
	GIQualityMiddleButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyGIQualityMiddle);
	GIQualityHighButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyGIQualityHigh);

	GIOptionButtons.Emplace(GIQualityLowButton);
	GIOptionButtons.Emplace(GIQualityMiddleButton);
	GIOptionButtons.Emplace(GIQualityHighButton);

	ShadingQualityLowButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyShadingQualityLow);
	ShadingQualityMiddleButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyShadingQualityMiddle);
	ShadingQualityHighButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyShadingQualityHigh);

	ShadingOptionButtons.Emplace(ShadingQualityLowButton);
	ShadingOptionButtons.Emplace(ShadingQualityMiddleButton);
	ShadingOptionButtons.Emplace(ShadingQualityHighButton);
	
	PPQualityLowButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyPPQualityLow);
	PPQualityMiddleButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyPPQualityMiddle);
	PPQualityHighButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyPPQualityHigh);

	PPOptionButtons.Emplace(PPQualityLowButton);
	PPOptionButtons.Emplace(PPQualityMiddleButton);
	PPOptionButtons.Emplace(PPQualityHighButton);
	
	AAQualityOffButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyAAQualityOff);
	AAQualityMiddleButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyAAQualityMiddle);
	AAQualityHighButton->OnClicked.AddDynamic(this, &ULLL_GraphicSettingWidget::ApplyAAQualityHigh);

	AAOptionButtons.Emplace(AAQualityOffButton);
	AAOptionButtons.Emplace(AAQualityMiddleButton);
	AAOptionButtons.Emplace(AAQualityHighButton);

	SetButtonStates();
}

void ULLL_GraphicSettingWidget::SetButtonStates()
{
	/* 그래픽 퀄리티는 0 - low, 1 - middle, 2 - High, 3- Epic 로 나뉘는데,
	 low는 품질 저하가 너무 심하기도 하고 현재 프로젝트 그래픽 수준이 높지 않아
	 Middle이 실질적인 low로 동작하게끔 함
	*/
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	TextureOptionButtons[Settings->GetTextureQuality() - 1]->OnClicked.Broadcast();
	ShadowOptionButtons[Settings->GetShadowQuality() - 1]->OnClicked.Broadcast();
	GIOptionButtons[Settings->GetGlobalIlluminationQuality() - 1]->OnClicked.Broadcast();
	ShadingOptionButtons[Settings->GetShadingQuality() - 1]->OnClicked.Broadcast();
	PPOptionButtons[Settings->GetPostProcessingQuality() - 1]->OnClicked.Broadcast();
	AAOptionButtons[Settings->GetAntiAliasingQuality() - 1]->OnClicked.Broadcast();
}

void ULLL_GraphicSettingWidget::ApplyTextureQualityLow()
{
	TextureQualityLowButton->SetIsEnabled(false);
	TextureQualityMiddleButton->SetIsEnabled(true);
	TextureQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetTextureQuality(static_cast<int32>(EGraphicQuality::Low));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyTextureQualityMiddle()
{
	TextureQualityLowButton->SetIsEnabled(true);
	TextureQualityMiddleButton->SetIsEnabled(false);
	TextureQualityHighButton->SetIsEnabled(true);

	GEngine->GetGameUserSettings()->SetTextureQuality(static_cast<int32>(EGraphicQuality::Middle));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyTextureQualityHigh()
{
	TextureQualityLowButton->SetIsEnabled(true);
	TextureQualityMiddleButton->SetIsEnabled(true);
	TextureQualityHighButton->SetIsEnabled(false);

	GEngine->GetGameUserSettings()->SetTextureQuality(static_cast<int32>(EGraphicQuality::High));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyShadowQualityLow()
{
	ShadowQualityLowButton->SetIsEnabled(false);
	ShadowQualityMiddleButton->SetIsEnabled(true);
	ShadowQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetShadowQuality(static_cast<int32>(EGraphicQuality::Low));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyShadowQualityMiddle()
{
	ShadowQualityLowButton->SetIsEnabled(true);
	ShadowQualityMiddleButton->SetIsEnabled(false);
	ShadowQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetShadowQuality(static_cast<int32>(EGraphicQuality::Middle));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyShadowQualityHigh()
{
	ShadowQualityLowButton->SetIsEnabled(true);
	ShadowQualityMiddleButton->SetIsEnabled(true);
	ShadowQualityHighButton->SetIsEnabled(false);
	
	GEngine->GetGameUserSettings()->SetShadowQuality(static_cast<int32>(EGraphicQuality::High));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyGIQualityLow()
{
	GIQualityLowButton->SetIsEnabled(false);
	GIQualityMiddleButton->SetIsEnabled(true);
	GIQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(static_cast<int32>(EGraphicQuality::Low));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyGIQualityMiddle()
{
	GIQualityLowButton->SetIsEnabled(true);
	GIQualityMiddleButton->SetIsEnabled(false);
	GIQualityHighButton->SetIsEnabled(false);
	
	GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(static_cast<int32>(EGraphicQuality::Middle));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyGIQualityHigh()
{
	GIQualityLowButton->SetIsEnabled(true);
	GIQualityMiddleButton->SetIsEnabled(true);
	GIQualityHighButton->SetIsEnabled(false);
	
	GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(static_cast<int32>(EGraphicQuality::High));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyShadingQualityLow()
{
	ShadingQualityLowButton->SetIsEnabled(false);
	ShadingQualityMiddleButton->SetIsEnabled(true);
	ShadingQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(static_cast<int32>(EGraphicQuality::Low));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyShadingQualityMiddle()
{
	ShadingQualityLowButton->SetIsEnabled(true);
	ShadingQualityMiddleButton->SetIsEnabled(false);
	ShadingQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(static_cast<int32>(EGraphicQuality::Middle));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyShadingQualityHigh()
{
	ShadingQualityLowButton->SetIsEnabled(true);
	ShadingQualityMiddleButton->SetIsEnabled(true);
	ShadingQualityHighButton->SetIsEnabled(false);
	
	GEngine->GetGameUserSettings()->SetGlobalIlluminationQuality(static_cast<int32>(EGraphicQuality::High));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyPPQualityLow()
{
	PPQualityLowButton->SetIsEnabled(false);
	PPQualityMiddleButton->SetIsEnabled(true);
	PPQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetPostProcessingQuality(static_cast<int32>(EGraphicQuality::Low));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyPPQualityMiddle()
{
	PPQualityLowButton->SetIsEnabled(true);
	PPQualityMiddleButton->SetIsEnabled(false);
	PPQualityHighButton->SetIsEnabled(true);
	
	GEngine->GetGameUserSettings()->SetPostProcessingQuality(static_cast<int32>(EGraphicQuality::Middle));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyPPQualityHigh()
{
	PPQualityLowButton->SetIsEnabled(true);
	PPQualityMiddleButton->SetIsEnabled(true);
	PPQualityHighButton->SetIsEnabled(false);
	
	GEngine->GetGameUserSettings()->SetPostProcessingQuality(static_cast<int32>(EGraphicQuality::High));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyAAQualityOff()
{
	AAQualityOffButton->SetIsEnabled(false);
	AAQualityMiddleButton->SetIsEnabled(true);
	AAQualityHighButton->SetIsEnabled(true);
	
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("r.AntiAliasingMethod 0"));
	GEngine->GetGameUserSettings()->SetAntiAliasingQuality(static_cast<int32>(EGraphicQuality::Low));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyAAQualityMiddle()
{
	AAQualityOffButton->SetIsEnabled(true);
	AAQualityMiddleButton->SetIsEnabled(false);
	AAQualityHighButton->SetIsEnabled(true);

	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("r.AntiAliasingMethod 1"));
	GEngine->GetGameUserSettings()->SetAntiAliasingQuality(static_cast<int32>(EGraphicQuality::Middle));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}

void ULLL_GraphicSettingWidget::ApplyAAQualityHigh()
{
	AAQualityOffButton->SetIsEnabled(true);
	AAQualityMiddleButton->SetIsEnabled(true);
	AAQualityHighButton->SetIsEnabled(false);

	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("r.AntiAliasingMethod 1"));
	GEngine->GetGameUserSettings()->SetAntiAliasingQuality(static_cast<int32>(EGraphicQuality::High));
	GEngine->GetGameUserSettings()->ApplySettings(true);
}