// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BossMonster/ManOfStrength/LLL_SnapOtherMonster_BTTaskNode.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"

ULLL_SnapOtherMonster_BTTaskNode::ULLL_SnapOtherMonster_BTTaskNode()
{
	NodeName = TEXT("Snap Other Monster");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_SnapOtherMonster_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_OTHER_MONSTER));
	if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
	{
		// Todo : 추후 데이터화 예정
		const FName SocketName = TEXT("hand_rSocket");
		if (!Monster->GetMesh()->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp, Log, TEXT("소켓이 존재하지 않습니다"))
			return EBTNodeResult::Failed;
		}

		CastChecked<ALLL_MonsterBaseAIController>(OtherMonster->GetController())->StopLogic(TEXT("Snapped"));
		OtherMonster->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OtherMonster->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		OtherMonster->AttachToComponent(Monster->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
