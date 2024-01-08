// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/LLL_MonsterBaseAIController.h"

#include "DataAsset/LLL_MonsterBaseDataAsset.h"

ALLL_MonsterBaseAIController::ALLL_MonsterBaseAIController()
{
	static ConstructorHelpers::FObjectFinder<ULLL_MonsterBaseDataAsset> DA_Monster(TEXT("/Script/Y2024Q1_Prototyping.LLL_MonsterDataAsset'/Game/DataAsset/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}
}

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 블랙보드와 행동트리 할당
	check(MonsterDataAsset);
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	
	if (UseBlackboard(MonsterDataAsset->MonsterBaseBlackBoard, BlackboardComponent))
	{
		check(RunBehaviorTree(MonsterDataAsset->MonsterBaseBehaviorTree));
	}
}
