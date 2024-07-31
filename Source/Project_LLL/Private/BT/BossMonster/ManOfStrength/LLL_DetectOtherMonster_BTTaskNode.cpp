// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BossMonster/ManOfStrength/LLL_DetectOtherMonster_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

ULLL_DetectOtherMonster_BTTaskNode::ULLL_DetectOtherMonster_BTTaskNode()
{
	NodeName = TEXT("Detect Other Monster");
}

EBTNodeResult::Type ULLL_DetectOtherMonster_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Monster);

	// Todo : 추후 데이터화 예정
	const float SnapRadius = 2000.0f;
	
	GetWorld()->SweepMultiByChannel(
		HitResults,
		Monster->GetActorLocation(),
		Monster->GetActorLocation(),
		FQuat::Identity,
		ECC_ENEMY,
		FCollisionShape::MakeSphere(SnapRadius),
		Params
		);

	for (auto HitResult : HitResults)
	{
		ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(HitResult.GetActor());
		if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_OTHER_MONSTER, OtherMonster);
			DrawDebugSphere(GetWorld(), Monster->GetActorLocation(), SnapRadius, 16, FColor::Green, false, 2.0f);
			return EBTNodeResult::Succeeded;
		}
	}
	
	DrawDebugSphere(GetWorld(), Monster->GetActorLocation(), SnapRadius, 16, FColor::Red, false, 2.0f);
	return EBTNodeResult::Failed;
}
