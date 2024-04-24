// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameInstance.h"

#include "Constant/LLL_FilePath.h"
#include "Util/LLL_ConstructorHelper.h"

ULLL_GameInstance::ULLL_GameInstance()
{
	FModParameterDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_FMOD_PARAMETER_NAME_DATA, EAssertionLevel::Check);
	
	AbilityDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_ABILITY_DATA_TABLE, EAssertionLevel::Check);

	RewardDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_REWARD_DATA_TABLE, EAssertionLevel::Check);
	
}

void ULLL_GameInstance::Init()
{
	Super::Init();

	TArray<FFModParameterDataTable*> LoadDataArray;
	FModParameterDataTable->GetAllRows<FFModParameterDataTable>(TEXT("Failed To Load FMod Parameter Name Data Tables"), LoadDataArray);

	for (const FFModParameterDataTable* LoadData : LoadDataArray)
	{
		FFModParameterDataTable TempData;
		TempData.Parameter = LoadData->Parameter;
		TempData.Name = LoadData->Name;
		FModParameterDataArray.Emplace(TempData);
	}

	TArray<FAbilityDataTable*> LoadAbilityDataArray;
	AbilityDataTable->GetAllRows<FAbilityDataTable>(TEXT("Failed To Load Ability Data Tables"), LoadAbilityDataArray);

	for (const FAbilityDataTable* LoadAbilityData : LoadAbilityDataArray)
	{
		FAbilityDataTable TempAbilityData;
		TempAbilityData.ID = LoadAbilityData->ID;
		TempAbilityData.AbilityType = LoadAbilityData->AbilityType;
		TempAbilityData.AbilityPart = LoadAbilityData->AbilityPart;
		TempAbilityData.AbilityRank = LoadAbilityData->AbilityRank;
		TempAbilityData.AbilityCategory = LoadAbilityData->AbilityCategory;
		TempAbilityData.AbilityName = LoadAbilityData->AbilityName;
		TempAbilityData.AbilityValue = LoadAbilityData->AbilityValue;
		TempAbilityData.ChangeValue = LoadAbilityData->ChangeValue;
		TempAbilityData.RequireCategory = LoadAbilityData->RequireCategory;
		
		AbilityData.Emplace(TempAbilityData);
	}

	TArray<FTestRewardDataTable*> LoadRewardDataArray;
	RewardDataTable->GetAllRows<FTestRewardDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadRewardDataArray);

	for (const FTestRewardDataTable* LoadRewardData : LoadRewardDataArray)
	{
		FTestRewardDataTable TempRewardData;
		TempRewardData.RewardType = LoadRewardData->RewardType;
		TempRewardData.RewardValue = LoadRewardData->RewardValue;
		TempRewardData.bIsHardReward = LoadRewardData->bIsHardReward;
		RewardData.Emplace(TempRewardData);
	}
}

