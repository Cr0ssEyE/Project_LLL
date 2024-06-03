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
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "Kismet/GameplayStatics.h"

ALLL_MapGimmick::ALLL_MapGimmick()
{
	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	RootBox->SetBoxExtent(FVector(5000.0f, 5000.0f, 500.0f));
	RootBox->SetCollisionProfileName(CP_OVERLAP_ALL);
	RootBox->OnComponentBeginOverlap.AddDynamic(this, &ALLL_MapGimmick::OnStageTriggerBeginOverlap);
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
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), nullptr, Settings, SequenceActorPtr);
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ALLL_MapGimmick::PlayerTeleport);
	PlayerTeleportNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld() ,MapDataAsset->TeleportParticle, FVector::ZeroVector, FRotator::ZeroRotator, MapDataAsset->ParticleScale, false, false);
}

void ALLL_MapGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void ALLL_MapGimmick::CreateMap()
{
	RoomActor = GetWorld()->SpawnActor<AActor>(RoomClass, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
	
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
			Gate->FadeOutDelegate.AddUObject(this, &ALLL_MapGimmick::FadeOut);
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
	
	RoomActor->GetAllChildActors(RoomChildActors, true);
	for (AActor* ChildActor : RoomChildActors)
	{
		if (ALLL_MonsterSpawner* Spawner = Cast<ALLL_MonsterSpawner>(ChildActor))
		{
			MonsterSpawner = Spawner;
		}
	}
	// TODO: Player loaction change
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorLocationAndRotation(PlayerSpawnPointComponent->GetComponentLocation(), PlayerSpawnPointComponent->GetComponentQuat());
	FadeIn();
	SetState(EStageState::READY);

	RootBox->SetCollisionProfileName(CP_OVERLAP_ALL);
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
		uint8 data = Seed;
		Seed = FMath::RandRange(0, MapDataAsset->Rooms.Num() - 1);
		if (Seed != data)
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

void ALLL_MapGimmick::OnInteractionGate(FRewardDataTable* Data)
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
	
}

void ALLL_MapGimmick::SetFight()
{
	RootBox->SetCollisionProfileName(CP_NO_COLLISION);
	OnOpponentSpawn();
}

void ALLL_MapGimmick::SetChooseReward()
{
	RootBox->SetCollisionProfileName(CP_NO_COLLISION);
	RewardSpawn();
}

void ALLL_MapGimmick::SetChooseNext()
{
	RootBox->SetCollisionProfileName(CP_NO_COLLISION);
	EnableAllGates();
}

void ALLL_MapGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	SetState(EStageState::REWARD);
}

void ALLL_MapGimmick::OnOpponentSpawn()
{
	if (MonsterSpawner)
	{
		MonsterSpawner->OnDestroyed.AddDynamic(this, &ALLL_MapGimmick::OnOpponentDestroyed);
	}
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
	ALLL_RewardObject* RewardObject = GetWorld()->SpawnActor<ALLL_RewardObject>(RewardObjectClass, Player->GetActorLocation(), Player->GetActorRotation());
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
}

void ALLL_MapGimmick::SetRewardWidget()
{
	RewardGimmick->SetRewardButtons();
}

void ALLL_MapGimmick::FadeIn()
{
	LevelSequenceActor->SetSequence(FadeInSequence);
	LevelSequencePlayer->Play();
}

void ALLL_MapGimmick::FadeOut()
{
	LevelSequenceActor->SetSequence(FadeOutSequence);
	LevelSequencePlayer->Play();
}

void ALLL_MapGimmick::PlayerTeleport()
{
	PlayerTeleportNiagara->SetWorldLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
	PlayerTeleportNiagara->ActivateSystem();
}
