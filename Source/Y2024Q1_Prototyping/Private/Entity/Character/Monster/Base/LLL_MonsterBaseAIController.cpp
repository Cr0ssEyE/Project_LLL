// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/LLL_MonsterDataAsset.h"

#include "Entity/Character/Monster/LLL_MonsterBaseAIController.h"

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 블랙보드와 행동트리 할당
	check(MonsterDataAsset);
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	
	UseBlackboard(MonsterDataAsset->MonsterBaseBlackBoard, BlackboardComponent);
	RunBehaviorTree(MonsterDataAsset->MonsterBaseBehaviorTree);
}
