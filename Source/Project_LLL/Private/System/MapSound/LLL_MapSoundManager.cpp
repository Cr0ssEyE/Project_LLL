// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapSound/LLL_MapSoundManager.h"

#include "FMODAudioComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void ALLL_MapSoundManager::SetPitch(float InPitch) const
{
	UFMODBlueprintStatics::EventInstanceSetParameter(BGMWrapper, FName(TEXT("Bullettime")), InPitch != 1.0f ? 1.0f : 0.0f);
	UFMODBlueprintStatics::EventInstanceSetPitch(AMBWrapper, InPitch);
}

void ALLL_MapSoundManager::SetBattleParameter(float Value) const
{
	UFMODBlueprintStatics::EventInstanceSetParameter(BGMWrapper, FName(TEXT("Battle")), Value);
}

void ALLL_MapSoundManager::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->CharacterDeadDelegate.AddDynamic(this, &ALLL_MapSoundManager::PlayerDeadHandle);

	BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), BGM, true);
	AMBWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), AMB, true);

	ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	GameInstance->SetMapSoundManager(this);
}

void ALLL_MapSoundManager::BeginDestroy()
{
	Super::BeginDestroy();

	UFMODBlueprintStatics::EventInstanceStop(BGMWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(BGMWrapper);
	
	UFMODBlueprintStatics::EventInstanceStop(AMBWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(AMBWrapper);
}

void ALLL_MapSoundManager::PlayerDeadHandle(ALLL_BaseCharacter* Character)
{
	UFMODBlueprintStatics::EventInstanceStop(BGMWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(BGMWrapper);
}
