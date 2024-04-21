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

	TArray<FTestAbilityDataTable*> LoadAbilityDataArray;
	AbilityDataTable->GetAllRows<FTestAbilityDataTable>(TEXT("Failed To Load FMod Parameter Name Data Tables"), LoadAbilityDataArray);

	for (const FTestAbilityDataTable* LoadDataTable : LoadAbilityDataArray)
	{
		FTestAbilityDataTable TempDataTable;
		TempDataTable.AbilityCategory = LoadDataTable->AbilityCategory;
		TempDataTable.AbilityParts = LoadDataTable->AbilityParts;
		TempDataTable.AbilityRank = LoadDataTable->AbilityRank;
		TempDataTable.AbilityType = LoadDataTable->AbilityType;
		TempDataTable.AbilityValue = LoadDataTable->AbilityValue;
		TempDataTable.ChangeValue = LoadDataTable->ChangeValue;
		TempDataTable.RequireCategory = LoadDataTable->RequireCategory;
		
		AbilityData.Emplace(TempDataTable);
	}

	TArray<FTestRewardDataTable*> LoadRewardDataTables;
	RewardDataTable->GetAllRows<FTestRewardDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadRewardDataTables);

	for (const FTestRewardDataTable* LoadDataTable : LoadRewardDataTables)
	{
		FTestRewardDataTable TempDataTable;
		TempDataTable.RewardType = LoadDataTable->RewardType;
		TempDataTable.RewardValue = LoadDataTable->RewardValue;
		TempDataTable.bIsHardReward = LoadDataTable->bIsHardReward;
		RewardData.Emplace(TempDataTable);
	}
}

