﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/LLL_ObjectBase.h"

#include "Components/BoxComponent.h"
#include "Util/LLLConstructorHelper.h"


// Sets default values
ALLL_ObjectBase::ALLL_ObjectBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BaseMesh->SetStaticMesh(FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"), EAssertionLevel::Check));
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	SetRootComponent(BaseMesh);
	CollisionBox->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ALLL_ObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALLL_ObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALLL_ObjectBase::DelayedDestroy(float Time)
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDelegate;
	DestroyTimerDelegate.BindUObject(this, &ALLL_ObjectBase::DestroyTimerCallback);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDelegate, Time, false);
}

void ALLL_ObjectBase::DelayedHide(float Time)
{
	FTimerHandle HideTimerHandle;
	FTimerDelegate HideTimerDelegate;
	HideTimerDelegate.BindUObject(this, &ALLL_ObjectBase::HideTimerCallback);
	GetWorldTimerManager().SetTimer(HideTimerHandle, HideTimerDelegate, Time, false);
}

void ALLL_ObjectBase::DestroyTimerCallback()
{
	Destroy();
}

void ALLL_ObjectBase::HideTimerCallback()
{
	SetActorHiddenInGame(true);
}

