// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ALLL_MonsterBaseAIController::ALLL_MonsterBaseAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	AISenseConfig_Sight = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	
	PerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
}

void ALLL_MonsterBaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Monster = CastChecked<ALLL_MonsterBase>(InPawn);
	MonsterDataAsset = CastChecked<ULLL_MonsterBaseDataAsset>(Monster->GetCharacterDataAsset());
	
	// 블랙보드와 행동트리 할당
	UBlackboardComponent* NewBlackboardComponent = GetBlackboardComponent();
	if (UseBlackboard(MonsterDataAsset->BlackBoard, NewBlackboardComponent))
	{
		check(RunBehaviorTree(MonsterDataAsset->BehaviorTree));
		BlackboardComponent = NewBlackboardComponent;
	}

	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = true;

	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_MonsterBaseAIController::AISenseInit);
	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_MonsterBaseAIController::MontageDelegateInit);
}

void ALLL_MonsterBaseAIController::AISenseInit() const
{
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	
	AISenseConfig_Sight->SightRadius = MonsterAttributeSet->GetDetectDistance();
	AISenseConfig_Sight->LoseSightRadius =  MonsterAttributeSet->GetDetectDistance();
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = MonsterAttributeSet->GetFieldOfView() / 2.0f;

	PerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
}

void ALLL_MonsterBaseAIController::MontageDelegateInit()
{
	Monster->GetCharacterAnimInstance()->OnMontageStarted.AddDynamic(this, &ALLL_MonsterBaseAIController::StartDamagedHandle);
	Monster->GetCharacterAnimInstance()->OnMontageEnded.AddDynamic(this, &ALLL_MonsterBaseAIController::EndDamagedHandle);
}

void ALLL_MonsterBaseAIController::StartDamagedHandle(UAnimMontage* Montage)
{
	if (Montage == MonsterDataAsset->DamagedAnimMontage)
	{
		BrainComponent->StopLogic("Monster Is Damaged");
	}
}

void ALLL_MonsterBaseAIController::EndDamagedHandle(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == MonsterDataAsset->DamagedAnimMontage)
	{
		BrainComponent->StartLogic();

		if (!IsValid(BlackboardComponent->GetValueAsObject(BBKEY_PLAYER)))
		{
			ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
			if (IsValid(Player))
			{
				BlackboardComponent->SetValueAsObject(BBKEY_PLAYER, Player);
			}
		}
	}
}
