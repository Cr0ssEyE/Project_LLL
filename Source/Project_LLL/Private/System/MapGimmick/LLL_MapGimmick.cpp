// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_MapGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Util/LLLConstructorHelper.h"
#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "System/MapGimmick/LLL_GateSpawnPointComponent.h"

// Sets default values
ALLL_MapGimmick::ALLL_MapGimmick()
{

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	RootBox->SetCollisionProfileName("NoCollision");
	SetRootComponent(RootBox);
	Stage = FLLLConstructorHelper::FindAndGetClass<AActor>(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/MapTest/BP_MapTest1234.BP_MapTest1234_C'"), EAssertionLevel::Check);

	GateClass = ALLL_GateObject::StaticClass();
	GateIndex = 0;
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

	CreateMapAndGate(0);
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
		if (!IsValid(SpawnPoint))
		{
			return;
		}
		AActor* GateActor = GetWorld()->SpawnActor<ALLL_GateObject>(GateClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
		ALLL_GateObject* Gate = Cast<ALLL_GateObject>(GateActor);
		if (Gate)
		{
			//Gate->GetRootComponent()->GetChildComponent(0);
			Gates.Add(Gate);
		}
	}
}

void ALLL_MapGimmick::RandomMap()
{
	
}

void ALLL_MapGimmick::OpenGates(uint8 index)
{
	Gates[index]->SetActorRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void ALLL_MapGimmick::CloseAllGates()
{
	
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


