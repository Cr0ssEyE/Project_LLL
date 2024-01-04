// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_InteractiveObject.h"

#include "Components/BoxComponent.h"

// Sets default values
ALLL_InteractiveObject::ALLL_InteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractOnlyCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Ineractive Collision"));
	InteractOnlyCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractOnlyCollisionBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALLL_InteractiveObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALLL_InteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

