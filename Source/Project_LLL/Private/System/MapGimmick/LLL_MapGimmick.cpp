// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_MapGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MapDataAsset.h"
#include "Util/LLLConstructorHelper.h"
#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"
#include "System/MapGimmick/LLL_GateSpawnPointComponent.h"

// Sets default values
ALLL_MapGimmick::ALLL_MapGimmick()
{

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	RootBox->SetCollisionProfileName("NoCollision");
	SetRootComponent(RootBox);

	MapDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MapDataAsset>(PATH_MAP_DATA, EAssertionLevel::Check);
	Stage = MapDataAsset->MapData[0];
	// State Section
	CurrentState = EStageState::READY;
	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &ALLL_MapGimmick::SetChooseNext)));

}

void ALLL_MapGimmick::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetState(CurrentState);
}

void ALLL_MapGimmick::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALLL_MapGimmick::BeginPlay()
{
	Super::BeginPlay();

	CreateMapAndGate();
}

void ALLL_MapGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void ALLL_MapGimmick::CreateMapAndGate()
{
	
	
	AActor* StageActor = GetWorld()->SpawnActor<AActor>(Stage, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
	for (USceneComponent* ChildComponent : StageActor->GetRootComponent()->GetAttachChildren())
	{
		ULLL_GateSpawnPointComponent* SpawnPoint = Cast<ULLL_GateSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			ALLL_GateObject* Gate = GetWorld()->SpawnActor<ALLL_GateObject>(ALLL_GateObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			Gates.Add(Gate);
		}
	}
}

void ALLL_MapGimmick::RandomMap()
{
	
}

void ALLL_MapGimmick::OpenAllGates()
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
	
}

void ALLL_MapGimmick::SetChooseReward()
{
	
}

void ALLL_MapGimmick::SetChooseNext()
{
	
}

void ALLL_MapGimmick::OnOpponentDestroyed(AActor* DestroyedActor)
{
	
}

void ALLL_MapGimmick::OnOpponentSpawn()
{
	
}


