// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameInstance.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_ShareableNiagaraDataAsset.h"
#include "Interface/LLL_FModInterface.h"
#include "Game/LLL_MapSoundSubsystem.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Materials/MaterialParameterCollection.h"
#include "Util/Save/LLL_CustomGameUserSettings.h"

ULLL_GameInstance::ULLL_GameInstance()
{
	FModParameterDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_FMOD_PARAMETER_NAME_DATA, EAssertionLevel::Check);
	AbilityDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_ABILITY_DATA_TABLE, EAssertionLevel::Check);
	RewardDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_REWARD_DATA_TABLE, EAssertionLevel::Check);
	StringDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_STRING_DATA, EAssertionLevel::Check);

	ShareableNiagaraDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ShareableNiagaraDataAsset>(PATH_SHAREABLE_NIAGARA_EFFECTS, EAssertionLevel::Check);
	GlobalParametersDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GlobalParameterDataAsset>(PATH_GLOBAL_PARAMETERS_DATA, EAssertionLevel::Check);
	
	MonsterMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_MONSTER_MPC, EAssertionLevel::Check);
	ObjectMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_OBJECT_MPC, EAssertionLevel::Check);
	PlayerMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_PLAYER_MPC, EAssertionLevel::Check);
	PostProcessMPC = FLLL_ConstructorHelper::FindAndGetObject<UMaterialParameterCollection>(PATH_POSTPROCESS_MPC, EAssertionLevel::Check);
	
	CustomTimeDilation = 1.f;
	CustomTimeDilationInterpSpeed = TIME_DILATION_INTERP_SPEED;
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

	GetWorld()->AddParameterCollectionInstance(MonsterMPC, true);
	GetWorld()->AddParameterCollectionInstance(ObjectMPC, true);
	GetWorld()->AddParameterCollectionInstance(PlayerMPC, true);
	GetWorld()->AddParameterCollectionInstance(PostProcessMPC, true);

	GetSubsystem<ULLL_MapSoundSubsystem>()->SetFModParameterDataArray(FModParameterData);
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]{
		GetSubsystem<ULLL_MapSoundSubsystem>()->PlayBGM();
	}));
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

	TArray<AActor*> SucceedActors;
	for (const auto Actor : Actors)
	{
		if (!IsValid(Actor))
		{
			continue;
		}

		if (Actor->CustomTimeDilation == InCustomTimeDilation)
		{
			SucceedActors.Emplace(Actor);
			continue;
		}
		
		Actor->CustomTimeDilation = CustomTimeDilation;
	
		if (const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(Actor))
		{
			FModInterface->GetFModAudioComponent()->SetPitch(CustomTimeDilation);
		}
	}

	GetSubsystem<ULLL_MapSoundSubsystem>()->SetBulletTimeParameterValue(CustomTimeDilation);

	for (auto Actor : SucceedActors)
	{
		if (Actors.Contains(Actor))
		{
			Actors.Remove(Actor);
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

