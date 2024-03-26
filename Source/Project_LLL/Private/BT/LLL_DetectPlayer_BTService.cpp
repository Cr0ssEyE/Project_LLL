// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/LLL_DetectPlayer_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
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
	
	ALLL_MonsterBase* MonsterBase = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const float FieldOfView = MonsterBase->GetFieldOfView();
	
	ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
	//DetectPlayer(OwnerComp, MonsterBase, FieldOfView);

	ALLL_MonsterBaseAIController* MonsterAIController = Cast<ALLL_MonsterBaseAIController>(MonsterBase->GetController());
	TArray<AActor*> OutActors;
	MonsterAIController->GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), OutActors);
	for (auto OutActor : OutActors)
	{
		if (Cast<ALLL_PlayerBase>(OutActor))
		{
			if (IsValid(PlayerBase))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, true);
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, OutActor);
			}
		}
	}
	
	if (PlayerBase->CheckCharacterIsDead())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, nullptr);
		return;
	}

	const bool IsInFieldOfView = IsPlayerInFieldOfView(MonsterBase, PlayerBase, FieldOfView) && LineOfSightToPlayer(MonsterBase, PlayerBase);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, IsInFieldOfView);
}

void ULLL_DetectPlayer_BTService::DetectPlayer(UBehaviorTreeComponent& OwnerComp, ALLL_MonsterBase* MonsterBase, float FieldOfView) const
{
	const float DetectDistance = MonsterBase->GetDetectDistance();
	const FVector Center = MonsterBase->GetActorLocation();
	const FVector Direction = MonsterBase->GetActorForwardVector();
	const float HalfFieldOfViewRadian = FMath::DegreesToRadians(FieldOfView / 2.0f);
	const FQuat Rot = FQuat::Identity;
	const FCollisionShape Shape = FCollisionShape::MakeSphere(DetectDistance);
		
	FColor DebugColor = FColor::Red;

	TArray<FOverlapResult> OverlapResults;
	GetWorld()->OverlapMultiByChannel(OverlapResults, Center, Rot, ECC_PLAYER_HIT, Shape);

	for (FOverlapResult const& OverlapResult : OverlapResults)
	{
		ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(OverlapResult.GetActor());
		if (!IsValid(PlayerBase) || PlayerBase->CheckCharacterIsDead())
		{
			continue;
		}

		if (!IsPlayerInFieldOfView(MonsterBase, PlayerBase, FieldOfView))
		{
			continue;
		}
		
		DebugColor = FColor::Green;
		
		if (LineOfSightToPlayer(MonsterBase, PlayerBase))
		{

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (ProtoGameInstance->CheckMonsterAttackDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 플레이어 감지"), *MonsterBase->GetName()));
				}
			}
#endif
							
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, PlayerBase);
			return;
		}
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckMonsterAttackDebug())
		{
			DrawDebugCone(GetWorld(), Center, Direction, DetectDistance, HalfFieldOfViewRadian, HalfFieldOfViewRadian, 16, DebugColor, false, 0.1f);
		}
	}
#endif
	
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
	
	if (GetWorld()->LineTraceSingleByProfile(HitResult, StartLocation, EndLocation, CP_MONSTER_ATTACK, CollisionParams))
	{
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterAttackDebug())
			{
				// 플레이어가 가려져 있음
				DrawDebugLine(GetWorld(), MonsterBase->GetActorLocation(), PlayerBase->GetActorLocation(), FColor::Red, false, 0.1f);
			}
		}
#endif
		
		return false;
	}

	return true;
}
