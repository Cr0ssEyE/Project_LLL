// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

#include "DataAsset/LLL_MonsterBaseDataAsset.h"

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 블랙보드와 행동트리 할당
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->BlackBoard, BlackboardComponent))
	{
		check(RunBehaviorTree(MonsterDataAsset->BehaviorTree));
	}
}
