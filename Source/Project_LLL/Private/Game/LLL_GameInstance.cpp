// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameInstance.h"

#include "Constant/LLL_FilePath.h"
#include "Interface/LLL_FModInterface.h"
#include "System/MapSound/LLL_MapSoundManager.h"
#include "Util/LLL_ConstructorHelper.h"

ULLL_GameInstance::ULLL_GameInstance()
{
	FModParameterDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_FMOD_PARAMETER_NAME_DATA, EAssertionLevel::Check);
	
	AbilityDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_ABILITY_DATA_TABLE, EAssertionLevel::Check);

	RewardDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_REWARD_DATA_TABLE, EAssertionLevel::Check);

	CustomTimeDilation = 1.0f;
	CustomTimeDilationInterpSpeed = 5.0f;
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
		TempAbilityData.AbilityValueType = LoadAbilityData->AbilityValueType;
		TempAbilityData.AbilityValue = LoadAbilityData->AbilityValue;
		TempAbilityData.ChangeValue = LoadAbilityData->ChangeValue;
		TempAbilityData.RequireCategory = LoadAbilityData->RequireCategory;
		AbilityData.Emplace(TempAbilityData);
	}
	
	TArray<FRewardDataTable*> LoadRewardDataArray;
	RewardDataTable->GetAllRows<FRewardDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadRewardDataArray);

	for (const FRewardDataTable* LoadRewardData : LoadRewardDataArray)
	{
		FRewardDataTable TempRewardData;
		TempRewardData.Value = LoadRewardData->Value;
		TempRewardData.DropType = LoadRewardData->DropType;
		TempRewardData.GetType = LoadRewardData->GetType;
		TempRewardData.GroupID = LoadRewardData->GroupID;
		TempRewardData.ID = LoadRewardData->ID;
		TempRewardData.UseType = LoadRewardData->UseType;
		TempRewardData.bIsHardReward = LoadRewardData->bIsHardReward;
		RewardData.Emplace(TempRewardData);
	}
}

void ULLL_GameInstance::SetActorsCustomTimeDilation(const TArray<AActor*>& Actors, float InCustomTimeDilation)
{
	if (!bCustomTimeDilationIsChanging)
	{
		bCustomTimeDilationIsChanging = true;
	
		SetActorsCustomTimeDilationRecursive(Actors, InCustomTimeDilation);
	}
}

void ULLL_GameInstance::SetActorsCustomTimeDilationRecursive(TArray<AActor*> Actors, float InCustomTimeDilation)
{
	for (const auto Actor : Actors)
	{
		Actor->CustomTimeDilation = CustomTimeDilation;
		UE_LOG(LogTemp, Log, TEXT("CustomTimeDilation : %.5f"), CustomTimeDilation)
	
		if (const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(Actor))
		{
			FModInterface->GetFModAudioComponent()->SetPitch(CustomTimeDilation);
		}

		if (const ALLL_MapSoundManager* MapSoundManager = Cast<ALLL_MapSoundManager>(Actor))
		{
			MapSoundManager->SetPitch(CustomTimeDilation);
		}
	}
	
	if (CustomTimeDilation == InCustomTimeDilation)
	{
		bCustomTimeDilationIsChanging = false;
		return;
	}

	CustomTimeDilation = FMath::FInterpTo(CustomTimeDilation, InCustomTimeDilation, GetWorld()->GetDeltaSeconds(), CustomTimeDilationInterpSpeed);

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, Actors, InCustomTimeDilation]{
		SetActorsCustomTimeDilationRecursive(Actors, InCustomTimeDilation);
	}));
}

