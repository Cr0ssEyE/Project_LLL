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
#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"
#include "System/MapGimmick/Components/LLL_GateSpawnPointComponent.h"
#include "System/MapGimmick/Components/LLL_ShoppingMapComponent.h"
#include "System/MapGimmick/Components/LLL_PlayerSpawnPointComponent.h"
#include "System/MonsterSpawner/LLL_MonsterSpawner.h"
#include "System/Reward/LLL_RewardGimmick.h"
#include "Util/LLL_ConstructorHelper.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "Game/LLL_GameInstance.h"
#include "Kismet/GameplayStatics.h"

ALLL_MapGimmick::ALLL_MapGimmick()
{
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	RootBox->SetCollisionProfileName(CP_OVERLAP_ALL);
	SetRootComponent(RootBox);

	MapDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_MapDataAsset>(PATH_MAP_DATA, EAssertionLevel::Check);
	RewardGimmick = CreateDefaultSubobject<ALLL_RewardGimmick>(TEXT("RewardGimmick"));
	CurrentState = EStageState::READY;

	FadeInSequence = MapDataAsset->FadeIn;
	FadeOutSequence = MapDataAsset->FadeOut;
	LevelSequenceActor = CreateDefaultSubobject<ALevelSequenceActor>(TEXT("SequenceActor"));

	Seed = 0;
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

	RewardGimmick->SetDataTable();
	RewardGimmick->InformMapGimmickIsExist();
	
	RewardData = RewardGimmick->GetRewardData(0);
	
	RandomMap();
	CreateMap();
	
	//Sequence Section
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;
	Settings.bPauseAtEnd = true;
	Settings.bHideHud = true;
	ALevelSequenceActor* SequenceActorPtr = LevelSequenceActor;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), FadeOutSequence, Settings, SequenceActorPtr);
	LevelSequencePlayer->Play();
}

void ALLL_MapGimmick::CreateMap()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorEnableCollision(false);
	
	RoomActor = GetWorld()->SpawnActor<AActor>(RoomClass, RootComponent->GetComponentTransform());
	
	for (USceneComponent* ChildComponent : RoomActor->GetRootComponent()->GetAttachChildren())
	{
		if (!IsValid(ShoppingMapComponent))
		{
			ShoppingMapComponent = Cast<ULLL_ShoppingMapComponent>(ChildComponent);
			if (IsValid(ShoppingMapComponent))
			{
				ShoppingMapComponent->ShopingDelegate.AddUObject(this, &ALLL_MapGimmick::SetRewardWidget);
			}
		}

		if (!IsValid(PlayerSpawnPointComponent))
		{
			PlayerSpawnPointComponent = Cast<ULLL_PlayerSpawnPointComponent>(ChildComponent);
		}

		const ULLL_GateSpawnPointComponent* SpawnPoint = Cast<ULLL_GateSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			ALLL_GateObject* Gate = GetWorld()->SpawnActor<ALLL_GateObject>(ALLL_GateObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			Gate->GateInteractionDelegate.AddUObject(this, &ALLL_MapGimmick::OnInteractionGate);
			RewardGimmick->SetRewardToGate(Gate);
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
	
	// TODO: Player loaction change 
	Player->SetActorLocationAndRotation(PlayerSpawnPointComponent->GetComponentLocation(), PlayerSpawnPointComponent->GetComponentQuat());
	Player->SetActorEnableCollision(true);
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
	AllGatesDestroy();
	RandomMap();
	CreateMap();
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
	RoomChildActors.Empty();
	if(IsValid(ShoppingMapComponent))
	{
		ShoppingMapComponent->DeleteProducts();
	}
	ShoppingMapComponent = nullptr;
	PlayerSpawnPointComponent = nullptr;
	RoomActor->Destroy();
}

void ALLL_MapGimmick::EnableAllGates()
{
	for (const auto Gate:Gates)
	{
		Gate->SetActivate();
	}
}

void ALLL_MapGimmick::SetState(EStageState InNewState)
{
	CurrentState = InNewState;

	if (StateChangeActions.Contains(InNewState))
	{
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void ALLL_MapGimmick::SetReady()
{
	UE_LOG(LogTemp, Log, TEXT("맵 상태 : %s"), *StaticEnum<EStageState>()->GetNameStringByValue(static_cast<int64>(CurrentState)));
}

void ALLL_MapGimmick::SetFight()
{
	UE_LOG(LogTemp, Log, TEXT("맵 상태 : %s"), *StaticEnum<EStageState>()->GetNameStringByValue(static_cast<int64>(CurrentState)));
	
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	GameInstance->SetMapSoundManagerBattleParameter(1.0f);
}

void ALLL_MapGimmick::SetChooseReward()
{
	UE_LOG(LogTemp, Log, TEXT("맵 상태 : %s"), *StaticEnum<EStageState>()->GetNameStringByValue(static_cast<int64>(CurrentState)));
	
	RewardSpawn();

	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	GameInstance->SetMapSoundManagerBattleParameter(0.0f);
}

void ALLL_MapGimmick::SetChooseNext()
{
	UE_LOG(LogTemp, Log, TEXT("맵 상태 : %s"), *StaticEnum<EStageState>()->GetNameStringByValue(static_cast<int64>(CurrentState)));
	
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
	RewardGimmick->SetRewardButtons();
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FTransform RewardTransform = Player->GetTransform();
	ALLL_RewardObject* RewardObject = GetWorld()->SpawnActorDeferred<ALLL_RewardObject>(RewardObjectClass, RewardTransform);
	if (IsValid(RewardObject))
	{
		RewardObject->SetInformation(RewardData);
		RewardObject->OnDestroyed.AddDynamic(this, &ALLL_MapGimmick::RewardDestroyed);
	}
	switch (RewardData->ID)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default: ;
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
}

void ALLL_MapGimmick::SetRewardWidget()
{
	RewardGimmick->SetRewardButtons();
}
