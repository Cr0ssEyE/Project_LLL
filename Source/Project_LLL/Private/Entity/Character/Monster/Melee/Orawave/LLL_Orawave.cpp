// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/Orawave/LLL_Orawave.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_OrawaveDataAsset.h"
#include "Entity/Character/Monster/Melee/Orawave/LLL_OrawaveAIController.h"
#include "GAS/Attribute/Monster/MeleeMonster/Orawave/LLL_OrawaveAttributeSet.h"
#include "Util/LLLConstructorHelper.h"

ALLL_Orawave::ALLL_Orawave()
{
	CharacterAttributeSet = CreateDefaultSubobject<ULLL_OrawaveAttributeSet>(TEXT("OrawaveAttributes"));
	
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_OrawaveDataAsset>(PATH_ORAWAVE_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_OrawaveAIController::StaticClass();
}

void ALLL_Orawave::BeginPlay()
{
	Super::BeginPlay();

	OrawaveMeleeDataAsset = Cast<ULLL_OrawaveDataAsset>(CharacterDataAsset);
}
