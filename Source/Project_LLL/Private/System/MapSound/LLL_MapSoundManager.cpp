// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapSound/LLL_MapSoundManager.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_GameInstance.h"
#include "Kismet/GameplayStatics.h"

ALLL_MapSoundManager::ALLL_MapSoundManager()
{
	PlayBGM = true;
}

void ALLL_MapSoundManager::SetPitch(float InPitch) const
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	for (const auto FModParameterData : GameInstance->GetFModParameterDataArray())
	{
		if (FModParameterData.Parameter == EFModParameter::BGM_BulletTimeParameter)
		{
			UFMODBlueprintStatics::EventInstanceSetParameter(BGMWrapper, FModParameterData.Name, InPitch != 1.0f ? 1.0f : 0.0f);
		}

		if (FModParameterData.Parameter == EFModParameter::AMB_BulletTimeParameter)
		{
			UFMODBlueprintStatics::EventInstanceSetParameter(AMBWrapper, FModParameterData.Name, InPitch != 1.0f ? 1.0f : 0.0f);
		}
	}
}

void ALLL_MapSoundManager::SetBattleParameter(float Value) const
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	for (const auto FModParameterData : GameInstance->GetFModParameterDataArray())
	{
		if (FModParameterData.Parameter != EFModParameter::BGM_BattleParameter)
		{
			continue;
		}

		UFMODBlueprintStatics::EventInstanceSetParameter(BGMWrapper, FModParameterData.Name, Value);
	}
}

void ALLL_MapSoundManager::SetPauseParameter(float Value) const
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	for (const auto FModParameterData : GameInstance->GetFModParameterDataArray())
	{
		if (FModParameterData.Parameter != EFModParameter::BGM_PauseParameter)
		{
			continue;
		}

		UFMODBlueprintStatics::EventInstanceSetParameter(BGMWrapper, FModParameterData.Name, Value);
	}
}

void ALLL_MapSoundManager::StopBGM() const
{
	UFMODBlueprintStatics::EventInstanceStop(BGMWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(BGMWrapper);
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

	if (PlayBGM)
	{
		BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), BGM, true);
	}
	AMBWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), AMB, true);

	ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	GameInstance->SetMapSoundManager(this);
}

void ALLL_MapSoundManager::BeginDestroy()
{
	Super::BeginDestroy();

	if (!IsValid(GetWorld()))
	{
		return;
	}

	StopBGM();
	
	UFMODBlueprintStatics::EventInstanceStop(AMBWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(AMBWrapper);
}

void ALLL_MapSoundManager::PlayerDeadHandle(ALLL_BaseCharacter* Character)
{
	StopBGM();

	UFMODBlueprintStatics::EventInstanceStop(AMBWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(AMBWrapper);
}
