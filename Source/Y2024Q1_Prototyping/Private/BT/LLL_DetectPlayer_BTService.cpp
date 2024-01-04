// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_DetectPlayer_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"

ULLL_DetectPlayer_BTService::ULLL_DetectPlayer_BTService()
{
	NodeName = TEXT("Detect Player");
	Interval = 0.5f;
}

void ULLL_DetectPlayer_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	const FVector Center = MonsterBase->GetActorLocation();
	const FQuat Rot = FQuat::Identity;
	FCollisionShape Shape = FCollisionShape::MakeSphere(400);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, MonsterBase);
	bool bResult = GetWorld()->OverlapMultiByProfile(OverlapResults, Center, Rot, TEXT("Monster"), Shape, CollisionQueryParam);

	if (bResult)
	{
		for (FOverlapResult const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerBase"), Pawn);
				DrawDebugSphere(GetWorld(), Center, 400, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(GetWorld(), Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(GetWorld(), MonsterBase->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerBase"), nullptr);
	DrawDebugSphere(GetWorld(), Center, 400, 16, FColor::Red, false, 0.2f);
}
