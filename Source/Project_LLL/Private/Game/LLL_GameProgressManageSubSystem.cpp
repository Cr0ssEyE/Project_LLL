// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameProgressManageSubSystem.h"

#include "Constant/LLL_GameplayInfo.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "Kismet/GameplayStatics.h"

ULLL_GameProgressManageSubSystem::ULLL_GameProgressManageSubSystem()
{
	
}

void ULLL_GameProgressManageSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (ULLL_SaveGameData* SaveData = Cast<ULLL_SaveGameData>(UGameplayStatics::LoadGameFromSlot(DEFAULT_FILE_NAME, DEFAULT_FILE_INDEX)))
	{
		CurrentSaveGameData = SaveData;
	}
}

void ULLL_GameProgressManageSubSystem::Deinitialize()
{
	if (IsValid(CurrentSaveGameData))
	{
		UGameplayStatics::SaveGameToSlot(CurrentSaveGameData, CurrentSaveGameData->SaveFileName, CurrentSaveGameData->SaveFileIndex);
	}
	
	Super::Deinitialize();
}

void ULLL_GameProgressManageSubSystem::InitializeGameProgressInfo(ULLL_SaveGameData* SaveGameData)
{
	ULLL_SaveGameData* SaveData = SaveGameData;
	if (SaveData == nullptr)
	{
		SaveData = GetCurrentSaveGameData();
	}

	if (SaveData != GetCurrentSaveGameData())
	{
		CurrentSaveGameData = SaveData;
	}

	// Do Something
}

void ULLL_GameProgressManageSubSystem::SaveGameProgressInfo()
{
	if (!IsValid(CurrentSaveGameData) || !IsValid(GetWorld()))
	{
		return;
	}

	SavePermanentData();
	SaveLastSessionMapData();
	SaveLastSessionPlayerData();
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGameData, CurrentSaveGameData->SaveFileName, CurrentSaveGameData->SaveFileIndex);
}

void ULLL_GameProgressManageSubSystem::SavePermanentData()
{
	// 영구 저장 계열 데이터. 게임 진행도, 세계수 강화, NPC 대화 진척도 등
}

void ULLL_GameProgressManageSubSystem::SaveLastSessionMapData()
{
	// = Lobby
	ALLL_MapGimmick* MapGimmick = CurrentInstanceMapGimmick.Get();
	if (!IsValid(MapGimmick))
	{
		// 로비의 현재 플레이어 위치 저장
		return;
	}

	FStageInfoData CurrentStageInfoData = MapGimmick->MakeStageInfoData();
	CurrentSaveGameData->StageInfoData = CurrentStageInfoData;
}

void ULLL_GameProgressManageSubSystem::SaveLastSessionPlayerData()
{
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UAbilitySystemComponent* PlayerASC = PlayerCharacter->GetAbilitySystemComponent();
	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	// 어빌리티 정보 저장
	TArray<int32> AcquiredEruriasID;
	TMap<int32, int32> AcquiredEruriasStackCount;
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
	PlayerASC->GetAllAbilities(AbilitySpecHandles);
	for (auto AbilitySpecHandle : AbilitySpecHandles)
	{
		FGameplayAbilitySpec* AbilitySpec = PlayerASC->FindAbilitySpecFromHandle(AbilitySpecHandle);
		ULLL_PGA_RewardAbilityBase* RewardAbility = Cast<ULLL_PGA_RewardAbilityBase>(AbilitySpec->Ability);
		if (!IsValid(RewardAbility))
		{
			continue;
		}

		AcquiredEruriasID.Emplace(RewardAbility->GetAbilityData()->ID);
		// 레벨로 했을 때 정상 작동 하는지 체크 필요
		AcquiredEruriasStackCount.Emplace(RewardAbility->GetAbilityData()->ID, RewardAbility->GetAbilityLevel());
	}

	CurrentSaveGameData->AcquiredEruriasID = AcquiredEruriasID;
	CurrentSaveGameData->AcquiredEruriasStackCount = AcquiredEruriasStackCount;
	
	// 어트리뷰트 정보 저장
	FPlayerCharacterStatusData PlayerCharacterStatusData = PlayerCharacterAttributeSet->MakeCharacterStatusData();
	CurrentSaveGameData->PlayerCharacterStatusData = PlayerCharacterStatusData;
	
	// 재화 정보 저장
	
}
