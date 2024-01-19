// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonsterAIController.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MeleeMonsterAIController::ALLL_MeleeMonsterAIController()
{
	MonsterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MeleeMonsterDataAsset>(PATH_MELEE_MONSTER_DATA, EAssertionLevel::Check);
}
