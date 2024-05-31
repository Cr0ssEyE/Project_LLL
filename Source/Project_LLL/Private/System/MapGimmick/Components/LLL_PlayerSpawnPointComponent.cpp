// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/Components/LLL_PlayerSpawnPointComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
ULLL_PlayerSpawnPointComponent::ULLL_PlayerSpawnPointComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("VisibleCollision"));
}


// Called when the game starts
void ULLL_PlayerSpawnPointComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULLL_PlayerSpawnPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

