// Fill out your copyright notice in the Description page of Project Settings.


#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

// Sets default values for this component's properties
ULLL_ObjectPoolingComponent::ULLL_ObjectPoolingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULLL_ObjectPoolingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULLL_ObjectPoolingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* ULLL_ObjectPoolingComponent::GetActor(UClass* Class)
{
	for (AActor* Actor : Actors)
	{
		if (Actor->IsHidden())
		{
			Actor->SetActorHiddenInGame(false);
			return Actor;
		}
	}

	AActor* NewActor = GetWorld()->SpawnActor(Class);
	Actors.Emplace(NewActor);
	return NewActor;
}

