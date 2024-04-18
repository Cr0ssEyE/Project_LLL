// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"

#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

ALLL_RangedMonster::ALLL_RangedMonster()
{
	ObjectPooling = CreateDefaultSubobject<ULLL_ObjectPoolingComponent>(TEXT("ObjectPooling"));
}

void ALLL_RangedMonster::BeginPlay()
{
	Super::BeginPlay();

	RangedMonsterDataAsset = Cast<ULLL_RangedMonsterDataAsset>(MonsterBaseDataAsset);
}
