// Copyright Epic Games, Inc. All Rights Reserved.

#include "Y2024Q1_PrototypingGameMode.h"
#include "Y2024Q1_PrototypingPlayerController.h"
#include "Y2024Q1_PrototypingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AY2024Q1_PrototypingGameMode::AY2024Q1_PrototypingGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AY2024Q1_PrototypingPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}