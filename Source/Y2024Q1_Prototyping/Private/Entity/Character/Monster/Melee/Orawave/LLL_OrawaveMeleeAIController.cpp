// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/Orawave/LLL_OrawaveMeleeAIController.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_OrawaveMeleeDataAsset.h"
#include "Util/LLLConstructorHelper.h"

ALLL_OrawaveMeleeAIController::ALLL_OrawaveMeleeAIController()
{
	MonsterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_OrawaveMeleeDataAsset>(PATH_ORAWAVE_MELEE_DATA, EAssertionLevel::Check);
}
