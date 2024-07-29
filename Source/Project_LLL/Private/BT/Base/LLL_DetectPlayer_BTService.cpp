// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/Base/LLL_DetectPlayer_BTService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Util/LLL_MathHelper.h"

ULLL_DetectPlayer_BTService::ULLL_DetectPlayer_BTService()
{
	NodeName = TEXT("Detect Player");
	Interval = 0.3f;
	RandomDeviation = 0.1f;
}

void ULLL_DetectPlayer_BTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (!IsValid(Player) || Player->CheckCharacterIsDead())
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, false);
		return;
	}

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetCharacter());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	const float DetectDistance = MonsterAttributeSet->GetDetectDistance();
	const float ProximityDetectDistance = MonsterAttributeSet->GetProximityDetectDistance();

	bool DetectPlayer = false;
	bool IsInFieldOfView = false;
	if (FLLL_MathHelper::IsInFieldOfView(Monster, Player, DetectDistance, MonsterAttributeSet->GetFieldOfView()))
	{
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Monster);
		CollisionParams.AddIgnoredActor(Player);
	
		FHitResult HitResult;
		if (!GetWorld()->LineTraceSingleByProfile(HitResult, Monster->GetActorLocation(), Player->GetActorLocation(), CP_MONSTER_ATTACK, CollisionParams))
		{
			DetectPlayer = true;
			IsInFieldOfView = true;
		}
	}
	else if (Monster->GetDistanceTo(Player) < MonsterAttributeSet->GetProximityDetectDistance())
	{
		DetectPlayer = true;
	}

	if (DetectPlayer && !Cast<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER)))
	{
		Monster->RecognizePlayerToAroundMonster();
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_PLAYER, Player);
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(BBKEY_IS_IN_FIELD_OF_VIEW, IsInFieldOfView);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			if (!OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER))
			{
				const FVector Direction = Monster->GetActorForwardVector();
				const float HalfFieldOfViewRadian = FMath::DegreesToRadians(MonsterAttributeSet->GetFieldOfView() / 2.0f);
				DrawDebugCone(GetWorld(), Monster->GetActorLocation(), Direction, DetectDistance, HalfFieldOfViewRadian, HalfFieldOfViewRadian, 16, FColor::Red, false, 0.1f);
				DrawDebugSphere(GetWorld(), Monster->GetActorLocation(), ProximityDetectDistance, 16, FColor::Red, false, 0.1f);
			}
		}
	}
#endif
}
