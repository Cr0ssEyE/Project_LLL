// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_MapGimmick.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MapDataAsset.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "System/MapGimmick/Components/LLL_GateSpawnPointComponent.h"
#include "System/MapGimmick/Components/LLL_ShoppingMapComponent.h"
#include "System/MapGimmick/Components/LLL_PlayerSpawnPointComponent.h"
#include "System/MonsterSpawner/LLL_MonsterSpawner.h"
#include "Game/LLL_RewardGimmickSubsystem.h"
#include "Util/LLL_ConstructorHelper.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "Game/LLL_GameInstance.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Game/LLL_MapSoundSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "System/MapGimmick/Components/LLL_SequencerComponent.h"

ALLL_MapGimmick::ALLL_MapGimmick()
{
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	RootBox->SetCollisionProfileName(CP_OVERLAP_ALL);
	SetRootComponent(RootBox);

	MapDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_MapDataAsset>(PATH_MAP_DATA, EAssertionLevel::Check);
	CurrentState = EStageState::READY;

	FadeInSequence = MapDataAsset->FadeIn;
	FadeOutSequence = MapDataAsset->FadeOut;
	FadeInSequenceActor = CreateDefaultSubobject<ALevelSequenceActor>(TEXT("SequenceActor"));

	Seed = 0;
	bIsFirstLoad = true;
}

FStageInfoData ALLL_MapGimmick::MakeStageInfoData()
{
	FStageInfoData StageInfoData;
	StageInfoData.Seed = Seed;
	StageInfoData.RoomNumber = CurrentRoomNumber;
	for (auto Gate : Gates)
	{
		ALLL_GateObject* GateObject = Gate.Get();
		if (!IsValid(GateObject))
		{
			continue;
		}
		StageInfoData.GatesRewardID.Emplace(GateObject->GetRewardData()->ID);
	}

	if (CurrentState != EStageState::READY && CurrentState != EStageState::FIGHT)
	{
		StageInfoData.LastStageState = CurrentState;
	}
	
	return StageInfoData;
}

void ALLL_MapGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void ALLL_MapGimmick::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Fight Section	
	MonsterSpawnerClass = MapDataAsset->MonsterSpawnerClass;

	//Reward Section
	RewardObjectClass = MapDataAsset->RewardObjectClass;
}

void ALLL_MapGimmick::BeginPlay()
{
	Super::BeginPlay();
	
	// State Section
	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetChooseNext)));
	RewardGimmickSubsystem = GetGameInstance()->GetSubsystem<ULLL_RewardGimmickSubsystem>();
	RewardGimmickSubsystem->SetDataTable();
	RewardGimmickSubsystem->InformMapGimmickIsExist();
	
	RewardData = RewardGimmickSubsystem->GetRewardData(0);

	PlayerTeleportNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld() ,MapDataAsset->TeleportParticle, FVector::ZeroVector, FRotator::ZeroRotator, MapDataAsset->ParticleScale, false, false);
	//Sequence Section
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;
	Settings.bPauseAtEnd = true;
	Settings.bHideHud = true;
	ALevelSequenceActor* SequenceActorPtr = FadeInSequenceActor;
	FadeInSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), FadeInSequence, Settings, SequenceActorPtr);
	FadeInSequencePlayer->OnFinished.AddDynamic(this, &ALLL_MapGimmick::PlayerTeleport);

	SequenceActorPtr = FadeOutSequenceActor;
	FadeOutSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), FadeOutSequence, Settings, SequenceActorPtr);
	FadeOutSequencePlayer->OnFinished.AddDynamic(this, &ALLL_MapGimmick::SetupGateData);
	
	SetupLevel();
	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->PlayBGM();
	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->PlayAMB();
	GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->RegisterMapGimmick(this);
	GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->OnLastSessionLoaded.AddDynamic(this, &ALLL_MapGimmick::LoadLastSessionMap);
	GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->LoadLastSessionMapData();
	// 델리게이트를 통해 마지막 세션 정보를 받아온 뒤, 세션 정보를 기반으로 진행도 초기화
}

void ALLL_MapGimmick::SetupLevel()
{
	for (auto Actor : GetWorld()->GetCurrentLevel()->Actors)
	{
		if (ALLL_GateObject* Gate = Cast<ALLL_GateObject>(Actor))
		{
			Gate->GateInteractionDelegate.AddUObject(this, &ALLL_MapGimmick::OnInteractionGate);
			RewardGimmickSubsystem->SetRewardToGate(Gate);
			Gates.Add(Gate);
		}
		
		if (ALLL_MonsterSpawner* Spawner = Cast<ALLL_MonsterSpawner>(Actor))
		{
			MonsterSpawner = Spawner;
			MonsterSpawner->StartSpawnDelegate.AddDynamic(this, &ALLL_MapGimmick::OnOpponentSpawn);
			MonsterSpawner->OnDestroyed.AddDynamic(this, &ALLL_MapGimmick::OnOpponentDestroyed);
		}
		SetState(EStageState::READY);
	}
}

void ALLL_MapGimmick::LoadLastSessionMap(FStageInfoData StageInfoData)
{
	if (GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->OnLastSessionLoaded.IsAlreadyBound(this, &ALLL_MapGimmick::LoadLastSessionMap))
	{
		GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->OnLastSessionLoaded.RemoveDynamic(this, &ALLL_MapGimmick::LoadLastSessionMap);
	}

	Seed = StageInfoData.Seed;
	CurrentRoomNumber = StageInfoData.RoomNumber;
	bIsLoadedFromSave = StageInfoData.bIsLoadedFromSave;

	// StageInfoData->GatesRewardID;

	/*// = 마지막 세션이 플레이 도중이 아님
	if (Seed == UINT32_MAX || CurrentRoomNumber == UINT32_MAX)
	{
		RandomMap();
	}
	else
	{
		if (CurrentRoomNumber == MapDataAsset->StoreRoom)
		{
			RoomClass = MapDataAsset->Store;
		}
		else if (CurrentRoomNumber > MapDataAsset->MaximumRoom)
		{
			RoomClass = MapDataAsset->Boss;
			GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->StopBGM();
		}
		else
		{
			RoomClass = MapDataAsset->Rooms[Seed];
		}
	}*/

	//ALLL_PlayerController* PlayerController = Cast<ALLL_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//PlayerController->PlayerInitializedDelegate.AddDynamic(this, &ALLL_MapGimmick::CreateMap);
}

void ALLL_MapGimmick::CreateMap()
{
	if (!IsValid(GetWorld()) || !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorEnableCollision(false);
	Player->SetActorHiddenInGame(true);
	Player->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	RoomActor = GetWorld()->SpawnActor<AActor>(RoomClass, RootComponent->GetComponentTransform());
	
	for (USceneComponent* ChildComponent : RoomActor->GetRootComponent()->GetAttachChildren())
	{
		if (!IsValid(ShoppingMapComponent))
		{
			ShoppingMapComponent = Cast<ULLL_ShoppingMapComponent>(ChildComponent);
			if (IsValid(ShoppingMapComponent))
			{
				ShoppingMapComponent->ShoppingDelegate.AddUObject(this, &ALLL_MapGimmick::SetRewardWidget);
			}
		}

		if (!IsValid(RoomSequencerPlayComponent))
		{
			RoomSequencerPlayComponent = Cast<ULLL_SequencerComponent>(ChildComponent);
		}
		
		if (!IsValid(PlayerSpawnPointComponent))
		{
			PlayerSpawnPointComponent = Cast<ULLL_PlayerSpawnPointComponent>(ChildComponent);
		}

		const ULLL_GateSpawnPointComponent* SpawnPoint = Cast<ULLL_GateSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			ALLL_GateObject* Gate = GetWorld()->SpawnActor<ALLL_GateObject>(MapDataAsset->Gate, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			Gate->GateInteractionDelegate.AddUObject(this, &ALLL_MapGimmick::OnInteractionGate);
			RewardGimmickSubsystem->SetRewardToGate(Gate);
			Gates.Add(Gate);
		}
	}

	RoomActor->OnDestroyed.AddDynamic(this, &ALLL_MapGimmick::ChangeMap);
	
	if (IsValid(ShoppingMapComponent))
	{
		ShoppingMapComponent->SetProducts();
		SetState(EStageState::NEXT);
	}
	else
	{
		RoomActor->GetAllChildActors(RoomChildActors, true);
		for (AActor* ChildActor : RoomChildActors)
		{
			if (ALLL_MonsterSpawner* Spawner = Cast<ALLL_MonsterSpawner>(ChildActor))
			{
				MonsterSpawner = Spawner;
				MonsterSpawner->StartSpawnDelegate.AddDynamic(this, &ALLL_MapGimmick::OnOpponentSpawn);
				MonsterSpawner->OnDestroyed.AddDynamic(this, &ALLL_MapGimmick::OnOpponentDestroyed);
			}
		}
		SetState(EStageState::READY);
	}

	bool PlayerTeleported = false;
	
	// 마지막 플레이 상태 적용
	if (bIsFirstLoad && bIsLoadedFromSave)
	{
		FStageInfoData LastInfoData = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->GetCurrentSaveGameData()->StageInfoData;
		UE_LOG(LogTemp, Log, TEXT("불러온 맵 상태 : %s"), *StaticEnum<EStageState>()->GetNameStringByValue(static_cast<int64>(LastInfoData.LastStageState)));
		CurrentState = LastInfoData.LastStageState;
		
		if (CurrentState == EStageState::REWARD || (CurrentState == EStageState::NEXT && !IsValid(ShoppingMapComponent)))
		{
			Player->SetActorLocation(LastInfoData.PlayerLocation);
			MonsterSpawner->OnDestroyed.Clear();
			MonsterSpawner->Destroy();
			PlayerTeleported = true;
		}

		if (CurrentState == EStageState::REWARD)
		{
			RewardSpawn();
		}
		else if (CurrentState == EStageState::NEXT && !IsValid(ShoppingMapComponent))
		{
			EnableAllGates();
		}
	}
	
	// TODO: Player location change
	if (!PlayerTeleported)
	{
		Player->SetActorLocationAndRotation(PlayerSpawnPointComponent->GetComponentLocation(), PlayerSpawnPointComponent->GetComponentQuat());
		ULLL_GameProgressManageSubSystem* GameProgressSubSystem = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>();
		if (IsValid(GameProgressSubSystem) && !bIsFirstLoad)
		{
			GameProgressSubSystem->BeginSaveGame();
		}
	}
	
	Player->SetActorEnableCollision(true);
	FadeIn();	
}

void ALLL_MapGimmick::RandomMap()
{
	CurrentRoomNumber++;
	if (CurrentRoomNumber == MapDataAsset->StoreRoom)
	{
		RoomClass = MapDataAsset->Store;
		return;
	}

	if (CurrentRoomNumber > MapDataAsset->MaximumRoom)
	{
		RoomClass = MapDataAsset->Boss;
		GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->StopBGM();
		return;
	}
	
	while (true)
	{
		const int32 Data = Seed;
		Seed = FMath::RandRange(0, MapDataAsset->Rooms.Num() - 1);
		if (Seed != Data)
		{
			break;
		}
	}
	RoomClass = MapDataAsset->Rooms[Seed];
}

void ALLL_MapGimmick::ChangeMap(AActor* DestroyedActor)
{
	// 이전 룸의 임시 데이터 초기화.
	if (!bIsFirstLoad)
	{
		ULLL_GameProgressManageSubSystem* GameProgressSubSystem = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>();
		GameProgressSubSystem->ClearInstantRoomData();
	}
	
	AllGatesDestroy();
	RandomMap();
	CreateMap();
}

void ALLL_MapGimmick::ChangeLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}

void ALLL_MapGimmick::AllGatesDestroy()
{
	if (Gates.Num() == 0)
	{
		return;
	}
	
	for	(const auto Gate : Gates)
	{
		Gate->Destroy();
	}
	Gates.Empty();
}

void ALLL_MapGimmick::OnInteractionGate(const FRewardDataTable* Data)
{
	RewardData = Data;
	bIsNextGateInteracted = true;
	PlayerTeleport();
}

void ALLL_MapGimmick::EnableAllGates()
{
	for (const auto Gate:Gates)
	{
		Gate->SetActivate();
	}
}

void ALLL_MapGimmick::SetupGateData()
{
	if (!bIsNextGateInteracted)
	{
		return;
	}

	/*RoomChildActors.Empty();
	if(IsValid(ShoppingMapComponent))
	{
		ShoppingMapComponent->DeleteProducts();
	}
	ShoppingMapComponent = nullptr;	
	PlayerSpawnPointComponent = nullptr;
	RoomSequencerPlayComponent = nullptr;
	
	RoomActor->Destroy();*/
	
	bIsNextGateInteracted = false;
	ChangeLevel();
}

void ALLL_MapGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;
	
	if (StateChangeActions.Contains(InNewState))
	{
		if (InNewState == EStageState::REWARD)
		{
			ULLL_GameProgressManageSubSystem* GameProgressSubSystem = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>();
			if (IsValid(GameProgressSubSystem) && !GameProgressSubSystem->CheckExitCurrentSession())
			{
				GameProgressSubSystem->BeginSaveGame();
			}
		}
		
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void ALLL_MapGimmick::SetReady()
{
	
}

void ALLL_MapGimmick::SetFight()
{
	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->SetBattleParameter(1.0f);
}

void ALLL_MapGimmick::SetChooseReward()
{
	RewardSpawn();

	GetGameInstance()->GetSubsystem<ULLL_MapSoundSubsystem>()->SetBattleParameter(0.0f);
}

void ALLL_MapGimmick::SetChooseNext()
{
	EnableAllGates();
}

void ALLL_MapGimmick::OnOpponentSpawn()
{
	SetState(EStageState::FIGHT);
}

void ALLL_MapGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::REWARD);
}

void ALLL_MapGimmick::RewardDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::NEXT);
}

void ALLL_MapGimmick::RewardSpawn()
{
	if (!UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		return;
	}
	RewardGimmickSubsystem->SetRewardButtons();
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTransform RewardTransform = Player->GetTransform();
	ALLL_AbilityRewardObject* RewardObject = GetWorld()->SpawnActor<ALLL_AbilityRewardObject>(ALLL_AbilityRewardObject::StaticClass(), RewardTransform);
	FVector Vector = RewardObject->GetActorLocation();
	Vector.Z += 100;
	RewardObject->SetActorLocation(Vector);
	if (IsValid(RewardObject))
	{
		RewardObject->SetInformation(RewardData);
		RewardObject->OnDestroyed.AddDynamic(this, &ALLL_MapGimmick::RewardDestroyed);
	}
	
	FHitResult Result;
	GetWorld()->SweepSingleByChannel
	(Result,
	Player->GetActorLocation(),
	Player->GetActorLocation(),
	FQuat::Identity,
	ECC_TRACE_FIELD,
	FCollisionShape::MakeBox(FVector(10.f, 10.f, 200.f))
	);
	
	if (!Result.GetActor())
	{
		RewardTransform.SetLocation(FVector::Zero() + FVector(0.f, 0.f, 300.f));
	}
	RewardObject->FinishSpawning(RewardTransform);

	ULLL_GameProgressManageSubSystem* GameProgressSubSystem = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>();
	if (bIsFirstLoad && GameProgressSubSystem->GetCurrentSaveGameData()->StageInfoData.RewardPosition != FVector::Zero())
	{
		RewardObject->SetActorLocation(GameProgressSubSystem->GetCurrentSaveGameData()->StageInfoData.RewardPosition);
	}
	RewardObjectPosition = RewardObject->GetActorLocation();
}

void ALLL_MapGimmick::SetRewardWidget()
{
	RewardGimmickSubsystem->SetRewardButtons();
}

void ALLL_MapGimmick::FadeIn()
{
	FadeInSequencePlayer->Play();

	FTimerHandle FadeTeleportTimerHandle;
	GetWorldTimerManager().SetTimer(FadeTeleportTimerHandle, this, &ALLL_MapGimmick::PlayerSetHidden, MapDataAsset->TeleportFadeOutDelay);
}

void ALLL_MapGimmick::FadeOut()
{
	FadeOutSequencePlayer->Play();
	PlayerSetHidden();
}

void ALLL_MapGimmick::PlayerTeleport()
{
	if (!IsValid(GetWorld()) || !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}

	if (IsValid(RoomSequencerPlayComponent) && RoomSequencerPlayComponent->CheckRoomEncounteredSequence())
	{
		PlayEncounterSequence();
		return;
	}
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerTeleportNiagara->SetWorldLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
	PlayerTeleportNiagara->ActivateSystem();

	if (bIsNextGateInteracted)
	{
		FTimerHandle FadeTeleportTimerHandle;
		GetWorldTimerManager().SetTimer(FadeTeleportTimerHandle, this, &ALLL_MapGimmick::FadeOut, MapDataAsset->TeleportFadeOutDelay);
	}
}

void ALLL_MapGimmick::PlayerSetHidden()
{
	if (!IsValid(GetWorld()) || !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player->IsHidden() && !bIsNextGateInteracted)
	{
		
		Player->SetActorHiddenInGame(false);
		Player->EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
	else
	{
		Player->SetActorHiddenInGame(true);
	}
	FadeInSequencePlayer->RestoreState();
	FadeOutSequencePlayer->RestoreState();

	if (bIsFirstLoad)
	{
		bIsFirstLoad = false;
	}
}

void ALLL_MapGimmick::PlayEncounterSequence()
{
	GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->EncounteredDelegate.Broadcast(RoomSequencerPlayComponent->GetRoomEncounterSequenceID());
}


