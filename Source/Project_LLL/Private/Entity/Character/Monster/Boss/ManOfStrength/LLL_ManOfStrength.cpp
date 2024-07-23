// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_Monster_Id.h"
#include "DataAsset/LLL_ManOfStrengthDataAsset.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ManOfStrength::ALLL_ManOfStrength()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ManOfStrengthDataAsset>(PATH_MAN_OF_STRENGTH_DATA, EAssertionLevel::Check);

	//임시로 다른 몬스터 데이터 들고오기
	//추후 변경 예정
	Id = ID_CLAW_BASIC;
}

void ALLL_ManOfStrength::BeginPlay()
{
	Super::BeginPlay();

	ManOfStrengthDataAsset = Cast<ULLL_ManOfStrengthDataAsset>(BossMonsterDataAsset);
}
