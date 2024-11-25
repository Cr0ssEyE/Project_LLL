// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerController.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Game/LLL_MapSoundSubsystem.h"
#include "Kismet/GameplayStatics.h"

ALLL_PlayerController::ALLL_PlayerController():
	bIsCharacterInitialized(false),
	bIsWidgetInitialized(false)
{
}

void ALLL_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::SetViewportMouseCaptureMode(this, EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	SetGameInputMode();
	FString Name = GetWorld()->GetName();
	if (Name == LEVEL_STAGE1_02_2 || Name == LEVEL_STAGE1_04 || Name == LEVEL_STAGE1_04_2)
	{
		return;
	}
	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->StopBGM();
	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->PlayBGM();
}

void ALLL_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(InPawn);
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->StartCameraMoveToCursor(this);
		WaitPlayerCharacterInitialize();
	}
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

void ALLL_PlayerController::WaitPlayerCharacterInitialize()
{
	if (bIsCharacterInitialized && bIsWidgetInitialized)
	{
		PlayerInitializedDelegate.Broadcast();
		PlayerInitializedDelegate.Clear();

		if (GetWorld()->GetName() != LEVEL_PROTOTYPE)
		{
			GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->LoadLastSessionPlayerData();
		}
		return;
	}
	GetWorldTimerManager().SetTimerForNextTick(this ,&ALLL_PlayerController::WaitPlayerCharacterInitialize);
}
