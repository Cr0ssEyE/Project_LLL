// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Lobby/LLL_LobbyManagementActor.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Entity/Object/Interactive/Lobby/LLL_DungeonEnteringObject.h"
#include "Entity/Object/Interactive/Lobby/LLL_WorldTreeObject.h"
#include "Kismet/GameplayStatics.h"
#include "System/Lobby/LLL_LobbyCustomPoint.h"

// Sets default values
ALLL_LobbyManagementActor::ALLL_LobbyManagementActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ALLL_LobbyManagementActor::BeginPlay()
{
	Super::BeginPlay();

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorEnableCollision(false);
	Player->SetActorHiddenInGame(true);
	Player->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	SetUpDefaultLobby();
	SetUpLobbyByGameProgress();
	SetUpLobbyByEventProgress();

	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_LobbyManagementActor::PlayerBeginEnter);
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
		DungeonEnteringObject->InteractionDelegate.AddDynamic(this, &ALLL_LobbyManagementActor::EnteringDungeon);
	}
	
	if (!IsValid(WorldTreeObject))
	{
		// 어지간하면 로비 레벨에 미리 셋업 해놓지만, 없는 경우 ID 기반으로 Custom Point 위치 찍어서 생성하기
	}

	if (IsValid(WorldTreeObject))
	{
		WorldTreeObject->InteractionDelegate.AddDynamic(this, &ALLL_LobbyManagementActor::WorldTreeInteractionEvent);
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

void ALLL_LobbyManagementActor::EnteringDungeon()
{
	// 던전 입장 연출
}

void ALLL_LobbyManagementActor::WorldTreeInteractionEvent()
{
	// 세계수 상호작용시 이벤트 발생하는 경우 여기서 처리
}

