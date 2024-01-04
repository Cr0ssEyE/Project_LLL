// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_DetectPlayer_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

ULLL_DetectPlayer_BTService::ULLL_DetectPlayer_BTService()
{
	NodeName = TEXT("Detect Player");
}

void ULLL_DetectPlayer_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	FHitResult HitResult;
	const FVector Start = MonsterBase->GetActorLocation();
	const FVector End = Start + MonsterBase->GetActorForwardVector() * 1000;
	const FQuat Rot = FQuat::Identity;
	FCollisionShape Shape = FCollisionShape::MakeSphere(500);
	GetWorld()->SweepSingleByProfile(HitResult, Start, End, Rot, TEXT("Monster"), Shape);
	
	ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(HitResult.GetActor());
	if (IsValid(PlayerBase))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerBase"), PlayerBase);
	}
}
