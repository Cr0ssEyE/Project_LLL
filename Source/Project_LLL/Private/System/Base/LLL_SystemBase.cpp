// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Base/LLL_SystemBase.h"

// Sets default values
ALLL_SystemBase::ALLL_SystemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALLL_SystemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALLL_SystemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

