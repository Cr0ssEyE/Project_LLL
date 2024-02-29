// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/Orawave/LLL_Orawave.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_OrawaveDataAsset.h"
#include "Entity/Character/Monster/Melee/Orawave/LLL_OrawaveAIController.h"
#include "Util/LLLConstructorHelper.h"

ALLL_Orawave::ALLL_Orawave()
{
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_OrawaveDataAsset>(PATH_ORAWAVE_DATA, EAssertionLevel::Check);
	OrawaveMeleeDataAsset = Cast<ULLL_OrawaveDataAsset>(CharacterDataAsset);
	if (IsValid(OrawaveMeleeDataAsset))
	{
		DetectDistance = OrawaveMeleeDataAsset->DetectDistance;
		FieldOfView = OrawaveMeleeDataAsset->FieldOfView;
	}

	AIControllerClass = ALLL_OrawaveAIController::StaticClass();
}
