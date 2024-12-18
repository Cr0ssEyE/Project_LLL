﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_TitleScreenWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBox.h"
#include "Constant/LLL_GameplayInfo.h"
#include "Constant/LLL_LevelNames.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/System/Setting/LLL_SettingWidget.h"
#include "Util/Save/LLL_SaveGameData.h"

void ULLL_TitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NewGameStartButton->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::NewGameStartButtonEvent);
	NewGameStartButton->OnHovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlayNewGameStartHoverAnimation);
	NewGameStartButton->OnUnhovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlayNewGameStartUnHoverAnimation);
	
	LoadGameButton->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::LoadGameButtonEvent);
	LoadGameButton->OnHovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlayLoadGameHoverAnimation);
	LoadGameButton->OnUnhovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlayLoadGameUnHoverAnimation);
	
	SettingButton->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::SettingButtonEvent);
	SettingButton->OnHovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlaySettingHoverAnimation);
	SettingButton->OnUnhovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlaySettingUnHoverAnimation);
	
	ExitGameButton->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::ExitGameButtonEvent);
	ExitGameButton->OnHovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlayExitGameHoverAnimation);
	ExitGameButton->OnUnhovered.AddDynamic(this, &ULLL_TitleScreenWidget::PlayExitGameUnHoverAnimation);
	
	ExitConfirmButton->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::ExitConfirmButtonEvent);
	ExitCancelButton->OnClicked.AddDynamic(this, &ULLL_TitleScreenWidget::ExitCancelButtonEvent);
	
	LobbyUIPanel->SetIsEnabled(false);
	ExitGameCheckPanel->SetRenderScale(FVector2D::Zero());
	SettingWidget->SetRenderScale(FVector2D::Zero());

	bool TestEnabled = false;
	
	if (bTestNoneSaveFileUI)
	{
		LoadGameButton->SetVisibility(ESlateVisibility::Hidden);
		LobbyButtonVerticalBox->RemoveChild(LoadGameButton);
		if (LobbyHoverVerticalBox->HasAnyChildren())
		{
			LobbyHoverVerticalBox->RemoveChildAt(1);
		}
		TestEnabled = true;
	}
	
	if (!TestEnabled && !IsValid(UGameplayStatics::LoadGameFromSlot(DEFAULT_FILE_NAME, DEFAULT_FILE_INDEX)))
	{
		LoadGameButton->SetVisibility(ESlateVisibility::Hidden);
		LobbyButtonVerticalBox->RemoveChild(LoadGameButton);

		if (LobbyHoverVerticalBox->HasAnyChildren())
		{
			LobbyHoverVerticalBox->RemoveChildAt(1);
		}
	}
	
	// 인트로 애니메이션 재생은 BP에서 함
}

void ULLL_TitleScreenWidget::NewGameStartButtonEvent()
{
	GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->CreateDefaultSaveSlot();
	DisableWidgetActivation();
}

void ULLL_TitleScreenWidget::LoadGameButtonEvent()
{
	DisableWidgetActivation();
	
	GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->InitializeGameProgressInfo();
}

void ULLL_TitleScreenWidget::SettingButtonEvent()
{
	const bool IsOpened = !SettingWidget->GetRenderTransform().Scale.X;
	SettingWidget->SetRenderScale(FVector2D::One() * IsOpened);
	if (IsOpened)
	{
		SettingWidget->SetIsEnabled(true);
	}
}

void ULLL_TitleScreenWidget::ExitGameButtonEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2D::One());
}

void ULLL_TitleScreenWidget::ExitConfirmButtonEvent()
{
	// 게임 종료 연출 들어갈지 안들어갈지 몰루는 상태
	
	CloseGame();
}

void ULLL_TitleScreenWidget::ExitCancelButtonEvent()
{
	ExitGameCheckPanel->SetRenderScale(FVector2D::Zero());
}

void ULLL_TitleScreenWidget::DisableWidgetActivation()
{
	SettingWidget->SetRenderScale(FVector2D::Zero());
	SettingWidget->SetIsEnabled(false);

	NewGameStartButton->OnClicked.Clear();
	LoadGameButton->OnClicked.Clear();
	SettingButton->OnClicked.Clear();
	ExitGameButton->OnClicked.Clear();

	ExitGameCheckPanel->SetRenderScale(FVector2D::Zero());
}

void ULLL_TitleScreenWidget::OpenIntroLevel()
{
	if (bIsLoadTestLevel)
	{
		UGameplayStatics::OpenLevel(this, LEVEL_PROLOGUE);
		return;
	}
	UGameplayStatics::OpenLevel(this, LEVEL_PROLOGUE);
}

void ULLL_TitleScreenWidget::OpenSavedLevel()
{
	const FName LastPlayedLevelName = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->GetLastPlayedLevelName();
	UGameplayStatics::OpenLevel(this, LastPlayedLevelName);
}

void ULLL_TitleScreenWidget::CloseGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}

