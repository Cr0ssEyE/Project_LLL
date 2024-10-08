// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/Base/LLL_MeleeMonster.h"

#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

ALLL_MeleeMonster::ALLL_MeleeMonster()
{
	AIControllerClass = ALLL_MonsterBaseAIController::StaticClass();
}

void ALLL_MeleeMonster::BeginPlay()
{
	Super::BeginPlay();

	MeleeMonsterDataAsset = Cast<ULLL_MeleeMonsterDataAsset>(MonsterBaseDataAsset);
}
