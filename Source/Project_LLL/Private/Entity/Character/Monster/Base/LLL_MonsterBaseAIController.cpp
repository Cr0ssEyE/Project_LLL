// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Monster = Cast<ALLL_MonsterBase>(InPawn);
	if (IsValid(Monster))
	{
		MonsterDataAsset = CastChecked<ULLL_MonsterBaseDataAsset>(Monster->GetCharacterDataAsset());

		Monster->TakeDamageDelegate.AddUObject(this, &ALLL_MonsterBaseAIController::SetPlayer);
	}

	// 블랙보드와 행동트리 할당
	BlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->BlackBoard, BlackboardComponent))
	{
		check(RunBehaviorTree(MonsterDataAsset->BehaviorTree));
	}
}

void ALLL_MonsterBaseAIController::SetPlayer()
{
	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (IsValid(Player))
	{
		BlackboardComponent->SetValueAsObject(BBKEY_PLAYER, Player);
	}
}
