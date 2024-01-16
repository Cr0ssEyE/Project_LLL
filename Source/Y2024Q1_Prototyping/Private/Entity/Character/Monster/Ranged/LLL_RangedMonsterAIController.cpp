// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAIController.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Util/LLLConstructorHelper.h"

ALLL_RangedMonsterAIController::ALLL_RangedMonsterAIController()
{
	MonsterBaseDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_RangedMonsterDataAsset>(PATH_RANGED_MONSTER_DATA, EAssertionLevel::Check);
}
