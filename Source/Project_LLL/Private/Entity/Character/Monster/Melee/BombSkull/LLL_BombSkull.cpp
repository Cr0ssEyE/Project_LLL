// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/BombSkull/LLL_BombSkull.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_Monster_Id.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_BombSkull::ALLL_BombSkull()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_BombSkullDataAsset>(PATH_BOMB_SKULL_DATA, EAssertionLevel::Check);

	Id = ID_BOMB_SKULL;
}

void ALLL_BombSkull::BeginPlay()
{
	Super::BeginPlay();

	BombSkullDataAsset = Cast<ULLL_BombSkullDataAsset>(MeleeMonsterDataAsset);
}

void ALLL_BombSkull::Attack()
{
	Dead();
}
