// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameInstance.h"

#include "Constant/LLL_FilePath.h"
#include "Game/LLL_MapSoundSubsystem.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "DataAsset/Global/LLL_GlobalParameterDataAsset.h"
#include "Materials/MaterialParameterCollection.h"
#include "Game/LLL_RewardGimmickSubsystem.h"
#include "Util/LLL_ConstructorHelper.h"

ULLL_GameInstance::ULLL_GameInstance()
{
	FModParameterDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_FMOD_PARAMETER_NAME_DATA, EAssertionLevel::Check);
	AbilityDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_ABILITY_DATA_TABLE, EAssertionLevel::Check);
	RewardDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_REWARD_DATA_TABLE, EAssertionLevel::Check);
	StringDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_STRING_DATA, EAssertionLevel::Check);

	GlobalNiagaraDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GlobalNiagaraDataAsset>(PATH_GLOBAL_NIAGARA_EFFECT_DATA, EAssertionLevel::Check);
	GlobalParametersDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GlobalParameterDataAsset>(PATH_GLOBAL_PARAMETERS_DATA, EAssertionLevel::Check);
	
	MonsterMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_MONSTER_MPC, EAssertionLevel::Check);
	ObjectMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_OBJECT_MPC, EAssertionLevel::Check);
	PlayerMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_PLAYER_MPC, EAssertionLevel::Check);
	PostProcessMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_POSTPROCESS_MPC, EAssertionLevel::Check);
}

void ULLL_GameInstance::Init()
{
	Super::Init();

	InitDataTables();

	GetWorld()->AddParameterCollectionInstance(MonsterMPC, true);
	GetWorld()->AddParameterCollectionInstance(ObjectMPC, true);
	GetWorld()->AddParameterCollectionInstance(PlayerMPC, true);
	GetWorld()->AddParameterCollectionInstance(PostProcessMPC, true);
	
	GetSubsystem<ULLL_MapSoundSubsystem>()->SetFModParameterDataArray(FModParameterData);
	/*GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]
	{
		GetSubsystem<ULLL_MapSoundSubsystem>()->PlayBGM();
	}));*/

	GetSubsystem<ULLL_RewardGimmickSubsystem>()->PlayerInitialize();
}

void ULLL_GameInstance::InitDataTables()
{
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
		if (!LoadAbilityData->bIsImplement)
		{
			continue;
		}
		AbilityData.Add(LoadAbilityData);
	}
	
	TArray<FRewardDataTable*> LoadRewardDataArray;
	RewardDataTable->GetAllRows<FRewardDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadRewardDataArray);

	for (const FRewardDataTable* LoadRewardData : LoadRewardDataArray)
	{
		RewardData.Add(LoadRewardData);
	}

	TArray<FStringDataTable*> LoadStringDataArray;
	StringDataTable->GetAllRows<FStringDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadStringDataArray);
	for (const FStringDataTable* LoadStringData : LoadStringDataArray)
	{
		StringData.Add(LoadStringData);
	}
}
