// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/StaffBasic/LLL_StaffBasic.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_Monster_Id.h"
#include "Entity/Character/Monster/Ranged/StaffBasic/LLL_StaffBasicAIController.h"
#include "GAS/Attribute/Character/Monster/RangedMonster/StaffBasic/LLL_StaffBasicAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_StaffBasic::ALLL_StaffBasic()
{
	CharacterAttributeSet = CreateDefaultSubobject<ULLL_StaffBasicAttributeSet>(TEXT("StaffBasicAttributeSet"));
	
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_StaffBasicDataAsset>(PATH_STAFF_BASIC_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_StaffBasicAIController::StaticClass();

	Id = ID_STAFF_BASIC;
}

void ALLL_StaffBasic::BeginPlay()
{
	Super::BeginPlay();

	StaffBasicDataAsset = Cast<ULLL_StaffBasicDataAsset>(RangedMonsterDataAsset);
}
