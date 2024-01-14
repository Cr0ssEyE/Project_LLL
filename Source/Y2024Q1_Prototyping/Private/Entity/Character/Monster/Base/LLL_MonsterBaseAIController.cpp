// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MonsterBaseAIController::ALLL_MonsterBaseAIController()
{
	MonsterBaseDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MonsterBaseDataAsset>(PATH_MONSTER_DATA, EAssertionLevel::Check);
}

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 블랙보드와 행동트리 할당
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterBaseDataAsset->BlackBoard, BlackboardComponent))
	{
		check(RunBehaviorTree(MonsterBaseDataAsset->BehaviorTree));
	}
}
