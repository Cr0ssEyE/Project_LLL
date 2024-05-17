// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/Base/LLL_RangedMonster.h"

#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

ALLL_RangedMonster::ALLL_RangedMonster()
{
	ObjectPoolingComponent = CreateDefaultSubobject<ULLL_ObjectPoolingComponent>(TEXT("ObjectPoolingComponent"));
}

void ALLL_RangedMonster::BeginPlay()
{
	Super::BeginPlay();

	RangedMonsterDataAsset = Cast<ULLL_RangedMonsterDataAsset>(MonsterBaseDataAsset);
}
