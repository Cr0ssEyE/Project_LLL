// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Lobby/LLL_DungeonEnteringObject.h"


// Sets default values
ALLL_DungeonEnteringObject::ALLL_DungeonEnteringObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bIsEnabled = true;
}

void ALLL_DungeonEnteringObject::InteractiveEvent(AActor* InteractedActor)
{
	if (!bIsEnabled)
	{
		return;
	}
	
	bIsEnabled = false;
	Super::InteractiveEvent(InteractedActor);
}