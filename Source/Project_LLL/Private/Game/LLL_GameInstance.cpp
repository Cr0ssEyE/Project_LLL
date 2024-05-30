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

	StringDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_STRING_DATA, EAssertionLevel::Check);
	
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
	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	for (const auto Actor : Actors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}
		
		Actor->CustomTimeDilation = CustomTimeDilation;
	
		if (const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(Actor))
		{
			FModInterface->GetFModAudioComponent()->SetPitch(CustomTimeDilation);
		}

		if (const ALLL_MapSoundManager* MapSoundManager = Cast<ALLL_MapSoundManager>(Actor))
		{
			MapSoundManager->SetPitch(CustomTimeDilation);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("CustomTimeDilation : %.5f"), CustomTimeDilation)
	
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

