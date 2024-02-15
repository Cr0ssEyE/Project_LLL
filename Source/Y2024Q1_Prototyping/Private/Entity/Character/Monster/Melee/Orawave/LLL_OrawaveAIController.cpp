// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/Orawave/LLL_OrawaveAIController.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_OrawaveDataAsset.h"
#include "Util/LLLConstructorHelper.h"

ALLL_OrawaveAIController::ALLL_OrawaveAIController()
{
	MonsterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_OrawaveDataAsset>(PATH_ORAWAVE_DATA, EAssertionLevel::Check);
}
