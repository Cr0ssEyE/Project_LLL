// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/Orawave/LLL_Orawave.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_OrawaveDataAsset.h"
#include "Entity/Character/Monster/Melee/Orawave/LLL_OrawaveAIController.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/Orawave/LLL_OrawaveAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_Orawave::ALLL_Orawave()
{
	CharacterAttributeSet = CreateDefaultSubobject<ULLL_OrawaveAttributeSet>(TEXT("OrawaveAttributeSet"));
	
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_OrawaveDataAsset>(PATH_ORAWAVE_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_OrawaveAIController::StaticClass();
}

void ALLL_Orawave::BeginPlay()
{
	Super::BeginPlay();

	OrawaveDataAsset = Cast<ULLL_OrawaveDataAsset>(MeleeMonsterDataAsset);
}
