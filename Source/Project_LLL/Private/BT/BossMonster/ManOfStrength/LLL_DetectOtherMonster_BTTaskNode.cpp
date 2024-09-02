// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BossMonster/ManOfStrength/LLL_DetectOtherMonster_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

ULLL_DetectOtherMonster_BTTaskNode::ULLL_DetectOtherMonster_BTTaskNode()
{
	NodeName = TEXT("Detect Other Monster");
}

EBTNodeResult::Type ULLL_DetectOtherMonster_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	const float DetectRadius = MonsterAttributeSet->GetMonsterData11();
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Monster);
	
	GetWorld()->SweepMultiByChannel(
		HitResults,
		Monster->GetActorLocation(),
		Monster->GetActorLocation(),
		FQuat::Identity,
		ECC_ENEMY,
		FCollisionShape::MakeSphere(DetectRadius),
		Params
		);

	FColor DebugColor = FColor::Red;
	bool Detected = false;
	for (auto HitResult : HitResults)
	{
		ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(HitResult.GetActor());
		if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_OTHER_MONSTER, OtherMonster);
			DebugColor =  FColor::Green;
			Detected = true;
		}
	}
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			DrawDebugSphere(GetWorld(), Monster->GetActorLocation(), DetectRadius, 16, DebugColor, false, 2.0f);
		}
	}
#endif

	if (Detected)
	{
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
