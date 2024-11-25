// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_MapSoundSubsystem.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_LevelNames.h"
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
	for (const auto FModParameterData : FModParameterDataArray)
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
	// 이게 없으면 종료할때 자꾸 터져서 일단 임시로 이렇게 해놨음
	// 추후 더 좋은 코드로 개선 예정
	if (this == nullptr)
	{
		return;
	}
	
	for (const auto FModParameterData : FModParameterDataArray)
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
	for (const auto FModParameterData : FModParameterDataArray)
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
	FString Name = GetWorld()->GetName();
	if(Name == LEVEL_TITLE)
	{
		BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MapSoundSubsystemDataAsset->BGMTitle, true);
		return;
	}
	if(Name == LEVEL_STAGE1_02)
	{
		BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MapSoundSubsystemDataAsset->BGMStage1, true);
		return;
	}
	if(Name == LEVEL_SHOP)
	{
		BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MapSoundSubsystemDataAsset->BGMShop, true);
		return;
	}
	if(Name == LEVEL_STAGE1_BOSS)
	{
		BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MapSoundSubsystemDataAsset->BGMStage1Boss, true);
		return;
	}
	if(Name == LEVEL_CREDIT)
	{
		BGMWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MapSoundSubsystemDataAsset->BGMDeer, true);
		return;
	}
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
