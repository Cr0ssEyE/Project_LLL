// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/ClawBasic/LLL_ClawBasic.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_Monster_Id.h"
#include "Entity/Character/Monster/Melee/ClawBasic/LLL_ClawBasicAIController.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/ClawBasic/LLL_ClawBasicAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ClawBasic::ALLL_ClawBasic()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ClawBasicDataAsset>(PATH_CLAW_BASIC_DATA, EAssertionLevel::Check);
	
	ClawBasicAttributeSet = CreateDefaultSubobject<ULLL_ClawBasicAttributeSet>(TEXT("ClawBasicAttributeSet"));
	AIControllerClass = ALLL_ClawBasicAIController::StaticClass();

	Id = ID_CLAW_BASIC;
}

void ALLL_ClawBasic::BeginPlay()
{
	Super::BeginPlay();

	ClawBasicDataAsset = Cast<ULLL_ClawBasicDataAsset>(MeleeMonsterDataAsset);
}
