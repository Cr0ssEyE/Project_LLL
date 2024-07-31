// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_GamePauseWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/System/Setting/LLL_SettingWidget.h"
#include "Game/LLL_MapSoundSubsystem.h"

void ULLL_GamePauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ResumeButton->OnClicked.AddDynamic(this, &ULLL_GamePauseWidget::ResumeButtonEvent);
	ResumeButton->OnHovered.AddDynamic(this, &ULLL_GamePauseWidget::PlayResumeButtonHoverAnimation);
	ResumeButton->OnUnhovered.AddDynamic(this, &ULLL_GamePauseWidget::PlayResumeButtonUnHoverAnimation);
	
	SettingButton->OnClicked.AddDynamic(this, &ULLL_GamePauseWidget::SettingButtonEvent);
	SettingButton->OnHovered.AddDynamic(this, &ULLL_GamePauseWidget::PlaySettingButtonHoverAnimation);
	SettingButton->OnUnhovered.AddDynamic(this, &ULLL_GamePauseWidget::PlaySettingButtonUnHoverAnimation);
	
	TitleButton->OnClicked.AddDynamic(this, &ULLL_GamePauseWidget::TitleButtonEvent);
	TitleButton->OnHovered.AddDynamic(this, &ULLL_GamePauseWidget::PlayTitleButtonHoverAnimation);
	TitleButton->OnUnhovered.AddDynamic(this, &ULLL_GamePauseWidget::PlayTitleButtonUnHoverAnimation);
	
	ExitButton->OnClicked.AddDynamic(this, &ULLL_GamePauseWidget::ExitButtonEvent);
	ExitButton->OnHovered.AddDynamic(this, &ULLL_GamePauseWidget::PlayExitButtonHoverAnimation);
	ExitButton->OnUnhovered.AddDynamic(this, &ULLL_GamePauseWidget::PlayExitButtonUnHoverAnimation);
	
	SettingWidget->SetRenderScale(FVector2d::Zero());
	SettingWidget->SetIsEnabled(false);
}

FReply ULLL_GamePauseWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if ((InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::BackSpace) && ResumeButton->GetIsEnabled())
	{
		if (SettingWidget->GetIsEnabled())
		{
			SettingWidget->CloseSettingWidget();
		}
		else
		{
			Cast<ALLL_PlayerBase>(GetOwningPlayerPawn())->GetPlayerUIManager()->TogglePauseWidget(false);
		}
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULLL_GamePauseWidget::SetupPauseState()
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
	SetKeyboardFocus();
	GetOwningPlayer()->DisableInput(GetOwningPlayer());
	Cast<ALLL_PlayerController>(GetOwningPlayer())->SetUIInputMode(GetCachedWidget());
	
	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->SetPauseParameter(1.0f);
}

void ULLL_GamePauseWidget::RestorePauseState(bool EnableInput)
{
	PlayAnimationForward(ResetAnim);
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
	if (EnableInput)
	{
		GetOwningPlayer()->EnableInput(GetOwningPlayer());
		Cast<ALLL_PlayerController>(GetOwningPlayer())->SetGameInputMode();
	}
	
	if (SettingWidget->GetIsEnabled())
	{
		SettingWidget->HideMainWidget();
	}
	
	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->SetPauseParameter(0.0f);
}

void ULLL_GamePauseWidget::SetupDeadStateLayout() const
{
	PauseTypeText->SetText(FText::FromString(TEXT("죽었습니다!")));
	PauseTypeText->SetColorAndOpacity(FSlateColor(FColor::Red));
	ResumeButton->SetIsEnabled(false);
	SettingButton->SetIsEnabled(false);
}

void ULLL_GamePauseWidget::ResumeButtonEvent()
{
	RestorePauseState();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void ULLL_GamePauseWidget::SettingButtonEvent()
{
	//TODO: 세팅 UI 만든 뒤에 붙이기
	SettingWidget->SetRenderScale(FVector2d::One());
	SettingWidget->SetIsEnabled(true);
	SettingWidget->ShowMainWidget();
}

void ULLL_GamePauseWidget::TitleButtonEvent()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	LastClickButton = TitleButton;
	PlayAnimationForward(FadeAnim);
}

void ULLL_GamePauseWidget::ExitButtonEvent()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	LastClickButton = ExitButton;
	PlayAnimationForward(FadeAnim);
}

void ULLL_GamePauseWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);
	
	if (Animation != FadeAnim)
	{
		return;
	}

	if (LastClickButton == TitleButton && Animation == FadeAnim)
	{
		UGameplayStatics::OpenLevel(this, LEVEL_TITLE);
		return;
	}

	if (LastClickButton == ExitButton && Animation == FadeAnim)
	{
		ULLL_GameProgressManageSubSystem* GameProgressSubSystem = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>();
		ULLL_SaveGameData* CurrentSaveGameData = GameProgressSubSystem->GetCurrentSaveGameData();
		if (IsValid(CurrentSaveGameData))
		{
			CurrentSaveGameData->LastPlayedLevelName = *GetWorld()->GetCurrentLevel()->GetName();
		}
	
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
	}
}
