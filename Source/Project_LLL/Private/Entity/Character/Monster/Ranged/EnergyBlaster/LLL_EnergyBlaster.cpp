// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/EnergyBlaster/LLL_EnergyBlaster.h"

#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Monster/Ranged/EnergyBlaster/LLL_EnergyBlasterAIController.h"
#include "GAS/Attribute/Monster/RangedMonster/EnergyBlaster/LLL_EnergyBlasterAttributeSet.h"
#include "Util/LLLConstructorHelper.h"

ALLL_EnergyBlaster::ALLL_EnergyBlaster()
{
	CharacterAttributeSet = CreateDefaultSubobject<ULLL_EnergyBlasterAttributeSet>(TEXT("EnergyBlasterAttributes"));
	
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_EnergyBlasterDataAsset>(PATH_ENERGYBLASTER_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_EnergyBlasterAIController::StaticClass();
}

void ALLL_EnergyBlaster::BeginPlay()
{
	Super::BeginPlay();

	EnergyBlasterDataAsset = Cast<ULLL_EnergyBlasterDataAsset>(CharacterDataAsset);
}
