// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_InteractiveTestActor.h"


// Sets default values
ALLL_InteractiveTestActor::ALLL_InteractiveTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsEnabled = false;
	bIsOnceEventOnly = false;
}

// Called when the game starts or when spawned
void ALLL_InteractiveTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALLL_InteractiveTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALLL_InteractiveTestActor::InteractiveEvent()
{
	Super::InteractiveEvent();
	
}

