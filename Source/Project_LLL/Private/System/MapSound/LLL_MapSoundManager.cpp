// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapSound/LLL_MapSoundManager.h"

#include "FMODAudioComponent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

void ALLL_MapSoundManager::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		return;
	}
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Player->CharacterDeadDelegate.AddDynamic(this, &ALLL_MapSoundManager::PlayerDeadHandle);

	BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), BGM, true);
	AMBWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), AMB, true);
}

void ALLL_MapSoundManager::PlayerDeadHandle(ALLL_BaseCharacter* Character)
{
	BGMWrapper.Instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	BGMWrapper.Instance->release();
	
	AMBWrapper.Instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
	AMBWrapper.Instance->release();
}
