// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/Orawave/LLL_OrawaveMelee.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_OrawaveMeleeDataAsset.h"
#include "Entity/Character/Monster/Melee/Orawave/LLL_OrawaveMeleeAIController.h"
#include "Util/LLLConstructorHelper.h"

ALLL_OrawaveMelee::ALLL_OrawaveMelee()
{
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_OrawaveMeleeDataAsset>(PATH_ORAWAVE_MELEE_DATA, EAssertionLevel::Check);
	OrawaveMeleeDataAsset = Cast<ULLL_OrawaveMeleeDataAsset>(CharacterDataAsset);
	if (IsValid(OrawaveMeleeDataAsset))
	{
		DetectDistance = OrawaveMeleeDataAsset->DetectDistance;
		FieldOfView = OrawaveMeleeDataAsset->FieldOfView;
	}

	AIControllerClass = ALLL_OrawaveMeleeAIController::StaticClass();
}
