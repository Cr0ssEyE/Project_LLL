// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerController.h"

#include "Kismet/GameplayStatics.h"

ALLL_PlayerController::ALLL_PlayerController()
{
	
}

void ALLL_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::SetViewportMouseCaptureMode(this, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	SetGameInputMode();
}

void ALLL_PlayerController::SetGameInputMode()
{
	FInputModeGameOnly GameOnlyInputMode;
	GameOnlyInputMode.SetConsumeCaptureMouseDown(false);
	SetShowMouseCursor(true);
	EnableInput(this);
	SetInputMode(GameOnlyInputMode);
}

void ALLL_PlayerController::SetUIInputMode(const TSharedPtr<SWidget>& FocusWidget)
{
	FInputModeUIOnly UIOnlyInputMode;
	if (FocusWidget.IsValid())
	{
		UIOnlyInputMode.SetWidgetToFocus(FocusWidget);
	}
	
	DisableInput(this);
	SetInputMode(UIOnlyInputMode);
}
