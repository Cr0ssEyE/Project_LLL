// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_TitleScreenWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Constant/LLL_LevelNames.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/System/Setting/LLL_SettingWidget.h"

void ULLL_TitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameStartBtn->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::GameStartBtnEvent);
	SettingBtn->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::SettingBtnEvent);
	ExitGameBtn->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::ExitGameBtnEvent);
	ConfirmBtn->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::ExitConfirmBtnEvent);
	CancelBtn->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::ExitCancelBtnEvent);
	
	LobbyUIPanel->SetIsEnabled(false);
	ExitGameCheckPanel->SetRenderScale(FVector2d::Zero());
	SettingWidget->SetRenderScale(FVector2d::Zero());
	
}

void ULLL_TitleScreenWidget::GameStartBtnEvent()
{
	FWidgetAnimationDynamicEvent AnimationEndDelegate;
	AnimationEndDelegate.BindDynamic(this, &ULLL_TitleScreenWidget::OpenIntroLevel);
	BindToAnimationFinished(LobbyFadeAnimation, AnimationEndDelegate);

	LobbyFadeAnimation.Get()->AnimationBindings.Empty();
	PlayAnimation(LobbyFadeAnimation);
}

void ULLL_TitleScreenWidget::SettingBtnEvent()
{
	bool IsOpened = !SettingWidget->GetRenderTransform().Scale.X;
	SettingWidget->SetRenderScale(FVector2d::One() * IsOpened);
}

void ULLL_TitleScreenWidget::ExitGameBtnEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2d::One());
}

void ULLL_TitleScreenWidget::ExitConfirmBtnEvent()
{
	SetLobbyUIActivation(false);
	FWidgetAnimationDynamicEvent AnimationEndDelegate;
	AnimationEndDelegate.BindDynamic(this, &ULLL_TitleScreenWidget::CloseGame);
	BindToAnimationFinished(LobbyFadeAnimation, AnimationEndDelegate);

	LobbyFadeAnimation.Get()->AnimationBindings.Empty();
	PlayAnimation(LobbyFadeAnimation);
}

void ULLL_TitleScreenWidget::ExitCancelBtnEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2d::Zero());
}

void ULLL_TitleScreenWidget::PlayShowLobbyUIAnimation()
{
	FWidgetAnimationDynamicEvent AnimationEndDelegate;
	AnimationEndDelegate.BindDynamic(this, &ULLL_TitleScreenWidget::OpenIntroLevel);
	BindToAnimationFinished(LobbyIntroAnimation, AnimationEndDelegate);

	LobbyIntroAnimation.Get()->AnimationBindings.Empty();
	PlayAnimation(LobbyIntroAnimation);
}

void ULLL_TitleScreenWidget::OpenIntroLevel()
{
	// TODO: 인트로 만들면 연결하기 + 게임 진행도에 따라 인트로 스킵 기능 만들기
	LobbyFadeAnimation.Get()->AnimationBindings.Empty();
	UGameplayStatics::OpenLevel(this, LEVEL_MAIN);
}

void ULLL_TitleScreenWidget::CloseGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

