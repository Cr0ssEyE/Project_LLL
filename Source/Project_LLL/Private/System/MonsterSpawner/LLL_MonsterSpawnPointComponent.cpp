// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MonsterSpawner/LLL_MonsterSpawnPointComponent.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MonsterSpawnerDataAsset.h"
#include "Util/LLL_ConstructorHelper.h"

ULLL_MonsterSpawnPointComponent::ULLL_MonsterSpawnPointComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	MonsterSpawnerDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_MonsterSpawnerDataAsset>(PATH_MONSTER_SPAWNER_DATA, EAssertionLevel::Check);
}

void ULLL_MonsterSpawnPointComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHiddenInGame(true);
}

