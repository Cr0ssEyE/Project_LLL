﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Lobby/LLL_WorldTreeObject.h"

ALLL_WorldTreeObject::ALLL_WorldTreeObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALLL_WorldTreeObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALLL_WorldTreeObject::InteractiveEvent(AActor* InteractedActor)
{
	Super::InteractiveEvent(InteractedActor);
	
}

