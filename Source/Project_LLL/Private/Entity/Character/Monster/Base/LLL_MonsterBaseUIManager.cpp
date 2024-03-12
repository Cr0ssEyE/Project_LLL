// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseUIManager.h"


// Sets default values for this component's properties
ULLL_MonsterBaseUIManager::ULLL_MonsterBaseUIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULLL_MonsterBaseUIManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULLL_MonsterBaseUIManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULLL_MonsterBaseUIManager::UpdateStatusWidget() const
{
	Super::UpdateStatusWidget();

	UE_LOG(LogTemp, Log, TEXT("asdf"));
}

