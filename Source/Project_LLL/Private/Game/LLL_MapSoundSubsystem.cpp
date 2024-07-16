// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_MapSoundSubsystem.h"

#include "Constant/LLL_FilePath.h"
#include "Game/LLL_GameInstance.h"
#include "Util/LLL_ConstructorHelper.h"

ULLL_MapSoundSubsystem::ULLL_MapSoundSubsystem() :
BGMWrapper(),
AMBWrapper()
{
	MapSoundSubsystemDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_MapSoundSubsystemDataAsset>(PATH_MAP_SOUND_SUBSYSTEM_DATA, EAssertionLevel::Check);
}

void ULLL_MapSoundSubsystem::SetBulletTimeParameterValue(float Value) const
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	for (const auto FModParameterData : GameInstance->GetFModParameterDataArray())
	{
		if (FModParameterData.Parameter == EFModParameter::BGM_BulletTimeParameter)
		{
			UFMODBlueprintStatics::EventInstanceSetParameter(BGMWrapper, FModParameterData.Name, Value != 1.0f ? 1.0f : 0.0f);
		}

		if (FModParameterData.Parameter == EFModParameter::AMB_BulletTimeParameter)
		{
			UFMODBlueprintStatics::EventInstanceSetParameter(AMBWrapper, FModParameterData.Name, Value != 1.0f ? 1.0f : 0.0f);
		}
	}
}

void ULLL_MapSoundSubsystem::SetBattleParameter(float Value) const
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

void ULLL_MapSoundSubsystem::SetPauseParameter(float Value) const
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

void ULLL_MapSoundSubsystem::PlayBGM()
{
	BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MapSoundSubsystemDataAsset->BGM, true);
}

void ULLL_MapSoundSubsystem::PlayAMB()
{
	AMBWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MapSoundSubsystemDataAsset->AMB, true);
}

void ULLL_MapSoundSubsystem::StopBGM() const
{
	UFMODBlueprintStatics::EventInstanceStop(BGMWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(BGMWrapper);
}

void ULLL_MapSoundSubsystem::StopAMB() const
{
	UFMODBlueprintStatics::EventInstanceStop(AMBWrapper);
	UFMODBlueprintStatics::EventInstanceRelease(AMBWrapper);
}

void ULLL_MapSoundSubsystem::PlayerDeadEvent() const
{
	StopBGM();
	StopAMB();
}

void ULLL_MapSoundSubsystem::BeginDestroy()
{
	Super::BeginDestroy();

	if (!IsValid(GetWorld()))
	{
		return;
	}

	StopBGM();
	StopAMB();
}
