// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_ThrownObject.h"
#include "Game/ProtoGameInstance.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_RangedMonster::ALLL_RangedMonster()
{
	ObjectPooling = CreateDefaultSubobject<ULLL_ObjectPoolingComponent>(TEXT("ObjectPooling"));
}

void ALLL_RangedMonster::BeginPlay()
{
	Super::BeginPlay();

	RangedMonsterDataAsset = Cast<ULLL_RangedMonsterDataAsset>(MonsterBaseDataAsset);
}
