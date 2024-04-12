// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDashAIController.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/SwordDash/LLL_SwordDashAttributeSet.h"
#include "Util/LLLConstructorHelper.h"

ALLL_SwordDash::ALLL_SwordDash()
{
	CharacterAttributeSet = CreateDefaultSubobject<ULLL_SwordDashAttributeSet>(TEXT("SwordDashAttributeSet"));

	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_SwordDashDataAsset>(PATH_SWORD_DASH_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_SwordDashAIController::StaticClass();
}

void ALLL_SwordDash::BeginPlay()
{
	Super::BeginPlay();

	SwordDashDataAsset = Cast<ULLL_SwordDashDataAsset>(MeleeMonsterDataAsset);
}
