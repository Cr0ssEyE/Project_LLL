// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/ClawBasic/LLL_ClawBasicAIController.h"


// Sets default values
ALLL_ClawBasicAIController::ALLL_ClawBasicAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALLL_ClawBasicAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALLL_ClawBasicAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

