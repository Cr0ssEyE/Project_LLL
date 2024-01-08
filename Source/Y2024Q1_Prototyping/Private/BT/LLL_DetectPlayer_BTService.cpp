// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_DetectPlayer_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

ULLL_DetectPlayer_BTService::ULLL_DetectPlayer_BTService()
{
	NodeName = TEXT("Detect Player");
	Interval = 0.5f;
}

void ULLL_DetectPlayer_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ALLL_MonsterBase* MonsterBase = Cast<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	const float DetectDistance = 700.0f;
	const float FieldOfView = 90.0f;
	
	const FVector Center = MonsterBase->GetActorLocation();
	const FVector Direction = MonsterBase->GetActorForwardVector();
	const float HalfFieldOfViewRadian = FMath::DegreesToRadians(FieldOfView / 2.0f);
	const FQuat Rot = FQuat::Identity;
	const FCollisionShape Shape = FCollisionShape::MakeSphere(DetectDistance);

	TArray<FOverlapResult> OverlapResults;
	if (GetWorld()->OverlapMultiByChannel(OverlapResults, Center, Rot, ECC_PLAYER_ONLY, Shape))
	{
		for (FOverlapResult const& OverlapResult : OverlapResults)
		{
			ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OverlapResult.GetActor());
			if (PlayerBase && PlayerBase->GetController()->IsPlayerController())
			{
				if (IsPlayerInFieldOfView(MonsterBase, PlayerBase, FieldOfView) && LineOfSightToPlayer(MonsterBase, PlayerBase))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, PlayerBase);
					DrawDebugCone(GetWorld(), Center, Direction, DetectDistance, HalfFieldOfViewRadian, HalfFieldOfViewRadian, 16, FColor::Green, false, 0.27f);

					DrawDebugPoint(GetWorld(), PlayerBase->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
					return;
				}
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, nullptr);
	DrawDebugCone(GetWorld(), Center, Direction, DetectDistance, HalfFieldOfViewRadian, HalfFieldOfViewRadian, 16, FColor::Red, false, 0.27f);
}

bool ULLL_DetectPlayer_BTService::IsPlayerInFieldOfView(const ALLL_MonsterBase* MonsterBase, const ALLL_PlayerBase* PlayerBase, float FieldOfView)
{
	FVector DirectionToPlayer = PlayerBase->GetActorLocation() - MonsterBase->GetActorLocation();
	DirectionToPlayer.Normalize();

	const FVector ForwardVector = MonsterBase->GetActorForwardVector();
	const float DotProduct = FVector::DotProduct(ForwardVector, DirectionToPlayer);

	if (FMath::Acos(DotProduct) <= FMath::DegreesToRadians(FieldOfView / 2))
	{
		// 시야각 내에 플레이어가 있음
		return true;
	}

	// 시야각 내에 플레이어가 없음
	return false;
}

bool ULLL_DetectPlayer_BTService::LineOfSightToPlayer(ALLL_MonsterBase* MonsterBase, ALLL_PlayerBase* PlayerBase) const
{
	FVector StartLocation = MonsterBase->GetActorLocation();
	FVector EndLocation = PlayerBase->GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(MonsterBase); // AI 자신은 무시
	CollisionParams.AddIgnoredActor(PlayerBase); // 플레이어는 무시
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		// 플레이어가 가려져 있음
		DrawDebugLine(GetWorld(), MonsterBase->GetActorLocation(), PlayerBase->GetActorLocation(), FColor::Red, false, 0.27f);
		return false;
	}

	// 플레이어가 가려져 있지 않음
	DrawDebugLine(GetWorld(), MonsterBase->GetActorLocation(), PlayerBase->GetActorLocation(), FColor::Green, false, 0.27f);
	return true;
}
