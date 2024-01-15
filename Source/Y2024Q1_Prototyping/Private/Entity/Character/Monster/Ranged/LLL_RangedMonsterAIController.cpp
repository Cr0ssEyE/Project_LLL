// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAIController.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Util/LLLConstructorHelper.h"

ALLL_RangedMonsterAIController::ALLL_RangedMonsterAIController()
{
	RangedMonsterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_RangedMonsterDataAsset>(PATH_RANGED_MONSTER_DATA, EAssertionLevel::Check);
}

void ALLL_RangedMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 블랙보드와 행동트리 할당
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(RangedMonsterDataAsset->BlackBoard, BlackboardComponent))
	{
		check(RunBehaviorTree(RangedMonsterDataAsset->BehaviorTree));
	}
}
