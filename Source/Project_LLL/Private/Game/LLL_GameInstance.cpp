// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameInstance.h"

#include "Constant/LLL_FilePath.h"
#include "Util/LLL_ConstructorHelper.h"

ULLL_GameInstance::ULLL_GameInstance()
{
	FModParameterDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_FMOD_PARAMETER_NAME_DATA, EAssertionLevel::Check);
	
	AbilityDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_ABILITY_DATA_TABLE, EAssertionLevel::Check);

	RewardDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_REWARD_DATA_TABLE, EAssertionLevel::Check);

	StringDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_STRING_DATA, EAssertionLevel::Check);
}

void ULLL_GameInstance::Init()
{
	Super::Init();

	TArray<FFModParameterDataTable*> LoadDataArray;
	FModParameterDataTable->GetAllRows<FFModParameterDataTable>(TEXT("Failed To Load FMod Parameter Name Data Tables"), LoadDataArray);

	for (const FFModParameterDataTable* LoadData : LoadDataArray)
	{
		FModParameterData.Add(*LoadData);
	}

	TArray<FAbilityDataTable*> LoadAbilityDataArray;
	AbilityDataTable->GetAllRows<FAbilityDataTable>(TEXT("Failed To Load Ability Data Tables"), LoadAbilityDataArray);

	for (const FAbilityDataTable* LoadAbilityData : LoadAbilityDataArray)
	{
		AbilityData.Add(*LoadAbilityData);
	}
	
	TArray<FRewardDataTable*> LoadRewardDataArray;
	RewardDataTable->GetAllRows<FRewardDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadRewardDataArray);

	for (const FRewardDataTable* LoadRewardData : LoadRewardDataArray)
	{
		RewardData.Add(*LoadRewardData);
	}

	TArray<FStringDataTable*> LoadStringDataArray;
	StringDataTable->GetAllRows<FStringDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadStringDataArray);
	for (const FStringDataTable* LoadStringData : LoadStringDataArray)
	{
		StringData.Add(*LoadStringData);
	}
}	

