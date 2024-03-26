// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ALLL_MonsterBaseAIController::ALLL_MonsterBaseAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	AISenseConfig_Sight = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));

	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;
	
	PerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
}

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Monster = CastChecked<ALLL_MonsterBase>(InPawn);
	if (IsValid(Monster))
	{
		MonsterDataAsset = CastChecked<ULLL_MonsterBaseDataAsset>(Monster->GetCharacterDataAsset());

		Monster->TakeDamageDelegate.AddUObject(this, &ALLL_MonsterBaseAIController::SetPlayer);
	}

	// 블랙보드와 행동트리 할당
	UBlackboardComponent* NewBlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->BlackBoard, NewBlackboardComponent))
	{
		check(RunBehaviorTree(MonsterDataAsset->BehaviorTree));
		BlackboardComponent = NewBlackboardComponent;
	}

	AISenseConfig_Sight->SightRadius = MonsterDataAsset->DetectDistance;
	AISenseConfig_Sight->LoseSightRadius = MonsterDataAsset->DetectDistance * 2;
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = MonsterDataAsset->FieldOfView / 2.0f;
}

void ALLL_MonsterBaseAIController::SetPlayer()
{
	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (IsValid(Player))
	{
		BlackboardComponent->SetValueAsObject(BBKEY_PLAYER, Player);
	}
}

void ALLL_MonsterBaseAIController::DetectCharacter(const TArray<AActor*>& UpdatedActors)
{
	for (auto Actor : UpdatedActors)
	{
		ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Actor);
		if (!IsValid(Player))
		{
			continue;
		}

		if (!IsValid(BlackboardComponent->GetValueAsObject(BBKEY_PLAYER)))
		{
			BlackboardComponent->SetValueAsObject(BBKEY_PLAYER, Player);
		}
		
		BlackboardComponent->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, true);
		return;
	}
	
	BlackboardComponent->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, false);
}
