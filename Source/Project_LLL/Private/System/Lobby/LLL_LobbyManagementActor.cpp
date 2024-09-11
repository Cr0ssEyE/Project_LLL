// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Lobby/LLL_LobbyManagementActor.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayInfo.h"
#include "Constant/LLL_LevelNames.h"
#include "DataAsset/Lobby/LLL_LobbyDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Entity/Object/Interactive/Lobby/LLL_DungeonEnteringObject.h"
#include "Entity/Object/Interactive/Lobby/LLL_WorldTreeObject.h"
#include "Game/LLL_GameInstance.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "System/Lobby/LLL_LobbyCustomPoint.h"
#include "Util/LLL_ConstructorHelper.h"

// Sets default values
ALLL_LobbyManagementActor::ALLL_LobbyManagementActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LobbyDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_LobbyDataAsset>(PATH_LOBBY_DATA, EAssertionLevel::Check);
}

// Called when the game starts or when spawned
void ALLL_LobbyManagementActor::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(UGameplayStatics::LoadGameFromSlot(DEFAULT_FILE_NAME, DEFAULT_FILE_INDEX)))
	{
		GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->CreateDefaultSaveSlot();
	}
	GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->InitializeLastSessionMapData(true);
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorEnableCollision(false);
	Player->SetActorHiddenInGame(true);
	Player->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (IsValid(LobbyDataAsset->TeleportParticle))
	{
		PlayerTeleportNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld() ,LobbyDataAsset->TeleportParticle, FVector::ZeroVector, FRotator::ZeroRotator, LobbyDataAsset->ParticleScale, false, false);
	}
	
	SetUpDefaultLobby();
	SetUpLobbyByGameProgress();
	SetUpLobbyByEventProgress();

	if (!IsValid(LobbySequenceActor))
	{
		LobbySequenceActor = GetWorld()->SpawnActorDeferred<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FTransform::Identity);
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = false;
		Settings.bHideHud = false;
		Settings.bDisableMovementInput = true;
		Settings.bDisableLookAtInput = true;
		Settings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
		
		LobbySequenceActor->PlaybackSettings = Settings;
		LobbySequenceActor->SetSequence(LobbyDataAsset->LobbyEnterSequence);
		LobbySequenceActor->GetSequencePlayer()->OnFinished.AddDynamic(this, &ALLL_LobbyManagementActor::PlayerBeginEnter);

		LobbySequenceActor->FinishSpawning(FTransform::Identity);
	}
	
	LobbySequenceActor->GetSequencePlayer()->Play();
}

void ALLL_LobbyManagementActor::SetUpDefaultLobby()
{
	for (auto Actor : GetWorld()->GetCurrentLevel()->Actors)
	{
		if (ALLL_LobbyCustomPoint* CustomPointActor = Cast<ALLL_LobbyCustomPoint>(Actor))
		{
			CustomPoints.Emplace(CustomPointActor);
			continue;
		}

		if (!IsValid(DungeonEnteringObject))
		{
			if (ALLL_DungeonEnteringObject* Object = Cast<ALLL_DungeonEnteringObject>(Actor))
			{
				DungeonEnteringObject = Object;
				continue;
			}
		}

		if (!IsValid(WorldTreeObject))
		{
			if (ALLL_WorldTreeObject* Object = Cast<ALLL_WorldTreeObject>(Actor))
			{
				WorldTreeObject = Object;
			}
		}
	}
	
	if (!IsValid(DungeonEnteringObject))
	{
		// 어지간하면 로비 레벨에 미리 셋업 해놓지만, 없는 경우 ID 기반으로 Custom Point 위치 찍어서 생성하기
	}

	if (IsValid(DungeonEnteringObject))
	{
		DungeonEnteringObject->OnInteractionDelegate.AddDynamic(this, &ALLL_LobbyManagementActor::PlayerEnteringDungeon);
	}
	
	if (!IsValid(WorldTreeObject))
	{
		// 어지간하면 로비 레벨에 미리 셋업 해놓지만, 없는 경우 ID 기반으로 Custom Point 위치 찍어서 생성하기
	}

	if (IsValid(WorldTreeObject))
	{
		WorldTreeObject->OnInteractionDelegate.AddDynamic(this, &ALLL_LobbyManagementActor::WorldTreeInteractionEvent);
	}
}

void ALLL_LobbyManagementActor::SetUpLobbyByGameProgress()
{
	// 추후 게임 진척도에 따른 로비 변화 적용
}

void ALLL_LobbyManagementActor::SetUpLobbyByEventProgress()
{
	// 추후 NPC 이벤트 등의 진척도에 따른 로비 변화 적용
}

void ALLL_LobbyManagementActor::PlayerBeginEnter()
{
	if (LobbySequenceActor->GetSequencePlayer()->OnFinished.IsAlreadyBound(this, &ALLL_LobbyManagementActor::PlayerBeginEnter))
	{
		LobbySequenceActor->GetSequencePlayer()->OnFinished.RemoveDynamic(this, &ALLL_LobbyManagementActor::PlayerBeginEnter);
	}
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player->DissolveCompleteDelegate.IsAlreadyBound(this, &ALLL_LobbyManagementActor::PlayerEnterComplete))
	{
		Player->DissolveCompleteDelegate.AddDynamic(this, &ALLL_LobbyManagementActor::PlayerEnterComplete);
	}
	
	Player->SetActorEnableCollision(true);
	Player->CharacterUnDissolveBegin();
}

void ALLL_LobbyManagementActor::PlayerEnterComplete(bool IsDrop)
{
	if (IsDrop)
	{
		return;
	}

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player->DissolveCompleteDelegate.IsAlreadyBound(this, &ALLL_LobbyManagementActor::PlayerEnterComplete))
	{
		Player->DissolveCompleteDelegate.RemoveDynamic(this, &ALLL_LobbyManagementActor::PlayerEnterComplete);
	}
	
	Player->EnableInput(Player->GetController<ALLL_PlayerController>());
}

void ALLL_LobbyManagementActor::UpdateLobbyHousing(ELobbyCustomPointType PointType, uint32 PointID)
{
	// 로비 상호작용 관련 요소 추가
}

void ALLL_LobbyManagementActor::PlayerEnteringDungeon()
{
	// 던전 입장 연출
	if (!IsValid(LobbySequenceActor))
	{
		LobbySequenceActor = GetWorld()->SpawnActorDeferred<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FTransform::Identity);
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = false;
		Settings.bHideHud = false;
		Settings.bDisableMovementInput = true;
		Settings.bDisableLookAtInput = true;
		Settings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceKeepState;
		
		LobbySequenceActor->PlaybackSettings = Settings;
		LobbySequenceActor->FinishSpawning(FTransform::Identity);
	}

	LobbySequenceActor->SetSequence(LobbyDataAsset->DungeonEnterSequence);
	LobbySequenceActor->GetSequencePlayer()->OnFinished.AddDynamic(this, &ALLL_LobbyManagementActor::OnDungeonEnterCompleted);
	LobbySequenceActor->GetSequencePlayer()->Play();

	if (IsValid(PlayerTeleportNiagara))
	{
		PlayerTeleportNiagara->SetWorldLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
		PlayerTeleportNiagara->ActivateSystem();
	}

	GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->InitializeLastSessionPlayData();
}

void ALLL_LobbyManagementActor::OnDungeonEnterCompleted()
{
	UGameplayStatics::OpenLevel(GetWorld(), LEVEL_TEST);
}

void ALLL_LobbyManagementActor::WorldTreeInteractionEvent()
{
	// 세계수 상호작용시 이벤트 발생하는 경우 여기서 처리
}

