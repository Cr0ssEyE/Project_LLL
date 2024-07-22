// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_ManOfStrengthDataAsset.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ManOfStrength::ALLL_ManOfStrength()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ManOfStrengthDataAsset>(PATH_MAN_OF_STRENGTH_DATA, EAssertionLevel::Check);

	//아이디 할당 필요
}

void ALLL_ManOfStrength::BeginPlay()
{
	Super::BeginPlay();

	ManOfStrengthDataAsset = Cast<ULLL_ManOfStrengthDataAsset>(BossMonsterDataAsset);
}
