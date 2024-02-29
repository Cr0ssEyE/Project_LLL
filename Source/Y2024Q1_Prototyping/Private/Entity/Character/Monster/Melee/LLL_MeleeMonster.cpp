// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"

#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

ALLL_MeleeMonster::ALLL_MeleeMonster()
{
	MeleeMonsterDataAsset = Cast<ULLL_MeleeMonsterDataAsset>(CharacterDataAsset);
}
