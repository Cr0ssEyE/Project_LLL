// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"

#include "Entity/Character/Monster/Boss/Base/LLL_BossMonsterAIController.h"

ALLL_BossMonster::ALLL_BossMonster()
{
	AIControllerClass = ALLL_BossMonsterAIController::StaticClass();
}

void ALLL_BossMonster::BeginPlay()
{
	Super::BeginPlay();

	BossMonsterDataAsset = Cast<ULLL_BossMonsterDataAsset>(MonsterBaseDataAsset);
}

void ALLL_BossMonster::Damaged(AActor* Attacker, bool IsDOT)
{
	ShowHitEffect();
	RecognizePlayerToAroundMonster();
}
