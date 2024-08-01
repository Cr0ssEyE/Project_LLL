// Fill out your copyright notice in the Description page of Project Settings.


#include "BT/BossMonster/ManOfStrength/LLL_ThrowOtherMonster_BTTaskNode.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

ULLL_ThrowOtherMonster_BTTaskNode::ULLL_ThrowOtherMonster_BTTaskNode()
{
	NodeName = TEXT("Throw Other Monster");
	bNotifyTick = true;
}

EBTNodeResult::Type ULLL_ThrowOtherMonster_BTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_OTHER_MONSTER));
	if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead())
	{
		FVector PlayerLocation = CastChecked<ALLL_PlayerBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER))->GetActorLocation();
		FVector Direction = (PlayerLocation - OtherMonster->GetActorLocation()).GetSafeNormal();

		// Todo : 추후 데이터화 예정
		float Speed = 1000.0f;
		OtherMonster->SetOwner(nullptr);
		OtherMonster->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		OtherMonster->GetMesh()->SetEnableGravity(false);
		OtherMonster->GetCapsuleComponent()->SetEnableGravity(false);
		OtherMonster->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OtherMonster->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OtherMonster->GetMesh()->SetCollisionProfileName(CP_THREW_MONSTER);
		OtherMonster->GetCapsuleComponent()->SetCollisionProfileName(CP_THREW_MONSTER);
		OtherMonster->GetProjectileMovementComponent()->Activate();
		OtherMonster->GetProjectileMovementComponent()->MaxSpeed = Speed;
		OtherMonster->GetProjectileMovementComponent()->Velocity = Direction * OtherMonster->GetProjectileMovementComponent()->MaxSpeed;
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_OTHER_MONSTER, nullptr);

		UE_LOG(LogTemp, Log, TEXT("%s 놓기"), *OtherMonster->GetName())
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
