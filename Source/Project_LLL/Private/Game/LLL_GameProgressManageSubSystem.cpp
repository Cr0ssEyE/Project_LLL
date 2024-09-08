﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameProgressManageSubSystem.h"

#include "Constant/LLL_GameplayInfo.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_AbilityManageSubSystem.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Effect/LLL_GE_GiveAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_GameProgressManageSubSystem::ULLL_GameProgressManageSubSystem() :
	bIsSavePermanentDataCompleted(false),
	bIsSaveMapDataCompleted(false),
	bIsSaveUserDataCompleted(false)
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

void ULLL_GameProgressManageSubSystem::CreateDefaultSaveSlot()
{
	ULLL_SaveGameData* NewData = Cast<ULLL_SaveGameData>(UGameplayStatics::CreateSaveGameObject(ULLL_SaveGameData::StaticClass()));
	UGameplayStatics::SaveGameToSlot(NewData, NewData->SaveFileName, NewData->SaveFileIndex);
	InitializeGameProgressInfo(NewData);
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

void ULLL_GameProgressManageSubSystem::InitializeLastSessionMapData(bool ResetToLobby) const
{
	if (ResetToLobby)
	{
		CurrentSaveGameData->LastPlayedLevelName = LEVEL_LOBBY;
	}
	
	FStageInfoData CurrentStageInfoData;
	CurrentSaveGameData->StageInfoData = CurrentStageInfoData;
}

void ULLL_GameProgressManageSubSystem::BeginSaveGame()
{
	if (!IsValid(CurrentSaveGameData) || !IsValid(GetWorld()))
	{
		if (IsValid(GetWorld()) && OnSaveCompleted.IsBound())
		{
			OnSaveCompleted.Broadcast();
		}
		return;
	}

	SavePermanentData();
	SaveLastSessionMapData();
	SaveLastSessionPlayerData();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_GameProgressManageSubSystem::SaveGameProgressInfo);
}

void ULLL_GameProgressManageSubSystem::SaveGameProgressInfo()
{
	if (bIsSaveMapDataCompleted && bIsSaveUserDataCompleted && bIsSavePermanentDataCompleted)
	{
		if (ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			if (PlayerCharacter->CheckCharacterIsDead())
			{
				CurrentSaveGameData->LastPlayedLevelName = LEVEL_LOBBY;
			}
		}
		
		UGameplayStatics::SaveGameToSlot(CurrentSaveGameData, CurrentSaveGameData->SaveFileName, CurrentSaveGameData->SaveFileIndex);
		OnSaveCompleted.Broadcast();

		bIsSaveMapDataCompleted = false;
		bIsSaveUserDataCompleted = false;
		bIsSavePermanentDataCompleted = false;
		return;
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_GameProgressManageSubSystem::SaveGameProgressInfo);
}

void ULLL_GameProgressManageSubSystem::LoadGameProgressInfo()
{
	
}

void ULLL_GameProgressManageSubSystem::LoadLastSessionMapData()
{
	if (!IsValid(CurrentSaveGameData) || !IsValid(GetWorld()) || !IsValid(CurrentInstanceMapGimmick.Get()))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (GetWorld()->GetName() == LEVEL_TEST)
		{
			const FStageInfoData TempStageInfoData;
			OnLastSessionLoaded.Broadcast(TempStageInfoData);
			return;
		}
#endif
		return;
	}

	if (GetLastPlayedLevelName() == LEVEL_LOBBY && GetWorld()->GetName() != LEVEL_TEST)
	{
		// 로비 관련 처리
		return;
	}

	OnLastSessionLoaded.Broadcast(CurrentSaveGameData->StageInfoData);
}

void ULLL_GameProgressManageSubSystem::LoadLastSessionPlayerData()
{
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(CurrentSaveGameData) || !IsValid(GetWorld()) || !IsValid(PlayerCharacter))
	{
		return;
	}
	
	if (GetLastPlayedLevelName() == LEVEL_LOBBY)
	{
		// 로비 관련 처리
		PlayerCharacter->SetActorLocation(CurrentSaveGameData->LobbyLastStandingLocation);
		return;
	}

	PlayerCharacter->InitAttributeSetBySave(&CurrentSaveGameData->PlayerCharacterStatusData);
	PlayerCharacter->GetGoldComponent()->IncreaseMoney(CurrentSaveGameData->CurrentGoldAmount);
	
	ULLL_AbilityManageSubSystem* AbilityManageSubSystem = GetGameInstance()->GetSubsystem<ULLL_AbilityManageSubSystem>();
	TArray<int32> PlayerAcquiredEruriasID = CurrentSaveGameData->AcquiredEruriasID;

	for (auto EruriaID : PlayerAcquiredEruriasID)
	{
		FAsyncLoadEffectByIDDelegate AsyncLoadEffectDelegate;
		AsyncLoadEffectDelegate.AddDynamic(this, &ULLL_GameProgressManageSubSystem::LoadLastSessionPlayerEruriaEffect);
		AbilityManageSubSystem->ASyncLoadEffectsByID(AsyncLoadEffectDelegate, EEffectOwnerType::Player, EruriaID, EEffectAccessRange::None);
	}
	
	OnLastSessionPlayerDataLoaded.Broadcast();
}

void ULLL_GameProgressManageSubSystem::LoadLastSessionPlayerEruriaEffect(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, int32 EffectID)
{
	TArray<const FAbilityDataTable*> EqualAbilities = FLLL_AbilityDataHelper::ApplyEruriaEffect(GetWorld(), LoadedEffects, EffectID);
	if (!EqualAbilities.IsEmpty())
	{
		for (auto EqualAbility : EqualAbilities)
		{
			CastChecked<ULLL_GameInstance>(GetGameInstance())->GetAbilityDataTable().Remove(EqualAbility);
		}
	}
}

void ULLL_GameProgressManageSubSystem::SavePermanentData()
{
	// 영구 저장 계열 데이터. 게임 진행도, 세계수 강화, NPC 대화 진척도 등
	bIsSavePermanentDataCompleted = true;
}

void ULLL_GameProgressManageSubSystem::SaveLastSessionMapData()
{
	if (!IsValid(GetWorld()))
	{
		return;
	}

	if (GetLastPlayedLevelName() == LEVEL_TUTORIAL)
	{
		bIsSaveMapDataCompleted = true;
		return;
	}
	
	CurrentSaveGameData->LastPlayedLevelName = *GetWorld()->GetName();
	ALLL_MapGimmick* MapGimmick = CurrentInstanceMapGimmick.Get();
	FStageInfoData CurrentStageInfoData;
	if (IsValid(MapGimmick))
	{
		CurrentStageInfoData = MapGimmick->MakeStageInfoData();
	}

	CurrentSaveGameData->StageInfoData = CurrentStageInfoData;

	bIsSaveMapDataCompleted = true;
}	

void ULLL_GameProgressManageSubSystem::SaveLastSessionPlayerData()
{
	// 로비인 경우 위치 정보만 저장
	if (GetLastPlayedLevelName() == LEVEL_LOBBY || GetLastPlayedLevelName() == LEVEL_TUTORIAL)
	{
		CurrentSaveGameData->LobbyLastStandingLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
		bIsSaveUserDataCompleted = true;
		return;
	}
	
	if (!IsValid(CurrentInstanceMapGimmick.Get()))
	{
		// 예외처리 필요
		bIsSaveUserDataCompleted = true;
		return;
	}
	
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter->CheckCharacterIsDead())
	{
		bIsSaveUserDataCompleted = true;
		return;
	}
	
	UAbilitySystemComponent* PlayerASC = PlayerCharacter->GetAbilitySystemComponent();
	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	// 어빌리티 정보 저장
	TArray<int32> AcquiredEruriasID;
	TMap<int32, int32> AcquiredEruriasStackCount;
	
	TArray<FGameplayEffectSpec> EffectSpecs;
	PlayerASC->GetAllActiveGameplayEffectSpecs(EffectSpecs);
	
	for (auto EffectSpec : EffectSpecs)
	{
		if (!EffectSpec.Def->FindComponent(ULLL_GE_GiveAbilityComponent::StaticClass()))
		{
			continue;
		}
		const ULLL_ExtendedGameplayEffect* RewardAbilityContainerEffect = Cast<ULLL_ExtendedGameplayEffect>(EffectSpec.Def);
		if (!IsValid(RewardAbilityContainerEffect))
		{
			continue;
		}

		AcquiredEruriasID.Emplace(RewardAbilityContainerEffect->GetAbilityData()->ID);
		// 레벨로 했을 때 정상 작동 하는지 체크 필요
		AcquiredEruriasStackCount.Emplace(RewardAbilityContainerEffect->GetAbilityData()->ID, EffectSpec.GetLevel());
	}

	CurrentSaveGameData->AcquiredEruriasID = AcquiredEruriasID;
	CurrentSaveGameData->AcquiredEruriasStackCount = AcquiredEruriasStackCount;
	
	// 어트리뷰트 정보 저장
	FPlayerCharacterStatusData PlayerCharacterStatusData = PlayerCharacterAttributeSet->MakeCharacterStatusData();
	CurrentSaveGameData->PlayerCharacterStatusData = PlayerCharacterStatusData;
	
	// 휘발성 재화 정보 저장
	CurrentSaveGameData->CurrentGoldAmount = PlayerCharacter->GetGoldComponent()->GetMoney();
	bIsSaveUserDataCompleted = true;
}
