// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_MapGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "Util/LLLConstructorHelper.h"

// Sets default values
ALLL_MapGimmick::ALLL_MapGimmick()
{
	// Stage Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	Stage->SetStaticMesh(FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/MapTest/Meshes/SM_MapTest.SM_MapTest'"), EAssertionLevel::Check));

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(1450.0, 1450.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CP_STAGETRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALLL_MapGimmick::OnStageTriggerBeginOverlap);

	// Gate Section
	static FName GateSockets[] = { TEXT("+XGate"), TEXT("-XGate"), TEXT("+YGate"), TEXT("-YGate") };

	GateClass = ALLL_GateObject::StaticClass();
	for (FName GateSocket : GateSockets)
	{
		FVector GateLocation = Stage->GetSocketLocation(GateSocket) + FVector(0.0f, 0.0f, 50.0f);
		GateLocations.Add(GateSocket, GateLocation);
		//AActor* GateActor = GetWorld()->SpawnActor(GateClass, &GateLocation, &FRotator::ZeroRotator);
		//GateActor->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
		//ALLL_GateObject* GateObjectActor = Cast<ALLL_GateObject>(GateActor);
		//Gates.Add(GateObjectActor);
	}

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

void ALLL_MapGimmick::BeginPlay()
{
	Super::BeginPlay();
}

void ALLL_MapGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT);
}

void ALLL_MapGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ALLL_MapGimmick::OpenAllGates()
{
	//게이트 상호작용 여부 변경
}

void ALLL_MapGimmick::CloseAllGates()
{
	//게이트 상호작용 여부 변경
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
	StageTrigger->SetCollisionProfileName(CP_STAGETRIGGER);
	
	CloseAllGates();
}

void ALLL_MapGimmick::SetFight()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	CloseAllGates();
}

void ALLL_MapGimmick::SetChooseReward()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	
	CloseAllGates();
}

void ALLL_MapGimmick::SetChooseNext()
{
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));

	OpenAllGates();
}


