// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_GamePauseWidget.h"

#include "Components/Button.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Kismet/GameplayStatics.h"

void ULLL_GamePauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeButton->OnClicked.AddDynamic(this, &ULLL_GamePauseWidget::ResumeButtonEvent);
	SettingButton->OnClicked.AddDynamic(this, &ULLL_GamePauseWidget::SettingButtonEvent);
	ExitButton->OnClicked.AddDynamic(this, &ULLL_GamePauseWidget::ExitButtonEvent);
}

void ULLL_GamePauseWidget::SetupDeadStateLayout() const
{
	ResumeButton->SetIsEnabled(false);
	SettingButton->SetIsEnabled(false);
}

void ULLL_GamePauseWidget::ResumeButtonEvent()
{
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}

void ULLL_GamePauseWidget::SettingButtonEvent()
{
	//TODO: 세팅 UI 만든 뒤에 붙이기
}

void ULLL_GamePauseWidget::ExitButtonEvent()
{
	//TODO: 정산창 Or 준비 공간으로 보내기
	ULLL_GameProgressManageSubSystem* SubSystem = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>();
	ULLL_SaveGameData* CurrentSaveGameData = SubSystem->GetCurrentSaveGameData();
	if (IsValid(CurrentSaveGameData))
	{
		CurrentSaveGameData->LastPlayLevelName = *GetWorld()->GetCurrentLevel()->GetName();
	}
	
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
