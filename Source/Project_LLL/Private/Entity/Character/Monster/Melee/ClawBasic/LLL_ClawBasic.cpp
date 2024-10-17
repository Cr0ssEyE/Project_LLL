// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/ClawBasic/LLL_ClawBasic.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_Monster_Id.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ClawBasic::ALLL_ClawBasic()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ClawBasicDataAsset>(PATH_CLAW_BASIC_DATA, EAssertionLevel::Check);

	Id = ID_CLAW_BASIC;
}

void ALLL_ClawBasic::BeginPlay()
{
	Super::BeginPlay();

	ClawBasicDataAsset = Cast<ULLL_ClawBasicDataAsset>(MeleeMonsterDataAsset);
}
