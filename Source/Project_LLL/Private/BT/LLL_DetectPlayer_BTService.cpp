// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_DetectPlayer_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Character/Monster/Base/LLL_MonsterAttributeSet.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

ULLL_DetectPlayer_BTService::ULLL_DetectPlayer_BTService()
{
	NodeName = TEXT("Detect Player");
	Interval = 0.3f;
	RandomDeviation = 0.1f;
}

void ULLL_DetectPlayer_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(OwnerComp.GetAIOwner());
	const ALLL_PlayerBase* BlackboardPlayer = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	bool IsHavePlayer = false;
	if (IsValid(BlackboardPlayer))
	{
		if (BlackboardPlayer->CheckCharacterIsDead())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, nullptr);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, false);
			return;
		}

		IsHavePlayer = true;
	}
	
	TArray<AActor*> OutActors;
	MonsterAIController->GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), OutActors);

	bool IsInFieldOfView = false;
	for (const auto OutActor : OutActors)
	{
		ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OutActor);
		if (IsValid(Player))
		{
			if (!IsHavePlayer)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, Player);
			}
			
			IsInFieldOfView = true;
			break;
		}
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, IsInFieldOfView);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckMonsterAttackDebug())
		{
			if (!OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER))
			{
				const ALLL_MonsterBase* MonsterBase = CastChecked<ALLL_MonsterBase>(MonsterAIController->GetPawn());
				const FVector Center = MonsterBase->GetActorLocation();
				const FVector Direction = MonsterBase->GetActorForwardVector();

				const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(MonsterBase->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
				const float DetectDistance = MonsterAttributeSet->GetDetectDistance();
				const float HalfFieldOfViewRadian = FMath::DegreesToRadians(MonsterAttributeSet->GetFieldOfView() / 2.0f);
				DrawDebugCone(GetWorld(), Center, Direction, DetectDistance, HalfFieldOfViewRadian, HalfFieldOfViewRadian, 16, FColor::Red, false, 0.1f);
			}
		}
	}
#endif
}
