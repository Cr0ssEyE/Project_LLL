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
	FInputModeGameOnly GameOnlyInputMode;
	GameOnlyInputMode.SetConsumeCaptureMouseDown(false);
	SetShowMouseCursor(true);
	SetInputMode(GameOnlyInputMode);
}
