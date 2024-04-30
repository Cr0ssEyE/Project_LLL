// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameInstance.h"

#include "Constant/LLL_FilePath.h"
#include "Util/LLL_ConstructorHelper.h"

ULLL_GameInstance::ULLL_GameInstance()
{
	FModParameterDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_FMOD_PARAMETER_NAME_DATA, EAssertionLevel::Check);
	
	AbilityDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_ABILITY_DATA_TABLE, EAssertionLevel::Check);

	RewardDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_REWARD_DATA_TABLE, EAssertionLevel::Check);

	ShopDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_SHOP_DATA_TABLE, EAssertionLevel::Check);
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
	AbilityDataTable->GetAllRows<FTestAbilityDataTable>(TEXT("Failed To Load Ability Data Tables"), LoadAbilityDataArray);

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
		TempDataTable.DropType = LoadDataTable->DropType;
		TempDataTable.GetType = LoadDataTable->GetType;
		TempDataTable.ID = LoadDataTable->ID;
		TempDataTable.UseType = LoadDataTable->UseType;
		TempDataTable.GroupID = LoadDataTable->GroupID;
		TempDataTable.Value = LoadDataTable->Value;
		TempDataTable.bIsHardReward = LoadDataTable->bIsHardReward;
		RewardData.Emplace(TempDataTable);
	}

	TArray<FTestShopDataTable*> LoadShopDataTables;
	ShopDataTable->GetAllRows<FTestShopDataTable>(TEXT("Failed To Load Shop Data Tables"), LoadShopDataTables);

	for (const FTestShopDataTable* LoadDataTable : LoadShopDataTables)
	{
		FTestShopDataTable TempDataTable;
		TempDataTable.AppearRate = LoadDataTable->AppearRate;
		TempDataTable.AppearStage = LoadDataTable->AppearStage;
		TempDataTable.ProductCount = LoadDataTable->ProductCount;
		TempDataTable.ProductPrice = LoadDataTable->ProductPrice;
		TempDataTable.RewardID = LoadDataTable->RewardID;
		TempDataTable.RewardGroupID = LoadDataTable->RewardGroupID;
		ShopData.Emplace(TempDataTable);
	}
}

