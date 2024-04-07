// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_MapGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MapDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Util/LLLConstructorHelper.h"
#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"
#include "Entity/Object/Interactive/LLL_RewardObject.h"
#include "System/MapGimmick/LLL_GateSpawnPointComponent.h"
#include "System/MonsterSpawner/LLL_MonsterSpawner.h"

// Sets default values
ALLL_MapGimmick::ALLL_MapGimmick()
{

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	RootBox->SetBoxExtent(FVector(5000.0f, 5000.0f, 500.0f));
	RootBox->SetCollisionProfileName(TEXT("OverlapAll"));
	RootBox->OnComponentBeginOverlap.AddDynamic(this, &ALLL_MapGimmick::OnStageTriggerBeginOverlap);
	SetRootComponent(RootBox);

	MapDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MapDataAsset>(PATH_MAP_DATA, EAssertionLevel::Check);

	CurrentState = EStageState::READY;
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
	
	RandomMap();
	CreateMap();
}

void ALLL_MapGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void ALLL_MapGimmick::CreateMap()
{
	AActor* StageActor = GetWorld()->SpawnActor<AActor>(Stage, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
	for (USceneComponent* ChildComponent : StageActor->GetRootComponent()->GetAttachChildren())
	{
		ULLL_GateSpawnPointComponent* SpawnPoint = Cast<ULLL_GateSpawnPointComponent>(ChildComponent);
		if (!IsValid(SpawnPoint))
		{
			ALLL_GateObject* Gate = GetWorld()->SpawnActor<ALLL_GateObject>(ALLL_GateObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			Gates.Add(Gate);
		}
	}
	
	TArray<AActor*> ChildActors;
	StageActor->GetAllChildActors(ChildActors, true);
	for (AActor* ChildActor : ChildActors)
	{
		MonsterSpawner = CastChecked<ALLL_MonsterSpawner>(ChildActor);
	}
}

void ALLL_MapGimmick::RandomMap()
{
	uint32 Seed = FMath::RandRange(0, MapDataAsset->MapData.Num() - 1);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Seed: %d"), Seed));
	Stage = MapDataAsset->MapData[Seed];
}

void ALLL_MapGimmick::EnableAllGates()
{
	for (auto Gate:Gates)
	{
		Gate->GateEnable();
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
	RootBox->SetCollisionProfileName(TEXT("NoCollision"));
	OnOpponentSpawn();
}

void ALLL_MapGimmick::SetChooseReward()
{
	RootBox->SetCollisionProfileName(TEXT("NoCollision"));
	RewardSpawn();
}

void ALLL_MapGimmick::SetChooseNext()
{
	RootBox->SetCollisionProfileName(TEXT("NoCollision"));
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
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ALLL_RewardObject* RewardObject = GetWorld()->SpawnActor<ALLL_RewardObject>(RewardObjectClass, Player->GetActorLocation(), Player->GetActorRotation());
	if (RewardObject)
	{
		RewardObject->OnDestroyed.AddDynamic(this, &ALLL_MapGimmick::RewardDestroyed);
	}
}


