// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

float ALLL_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHealthAmount <= 0)
	{
		Dead();
	}
	else
	{
		Stun();
	}
	
	return 0;
}

void ALLL_MonsterBase::Dead()
{
	Super::Dead();

	ALLL_MonsterBaseAIController* MonsterBaseAIController = Cast<ALLL_MonsterBaseAIController>(GetController());
	if (IsValid(MonsterBaseAIController))
	{
		MonsterBaseAIController->StopMovement();
		MonsterBaseAIController->GetBrainComponent()->StopLogic("Monster Is Dead");
	}
}

void ALLL_MonsterBase::Attack()
{
	Super::Attack();

	ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(MonsterBaseAnimInstance))
	{
		MonsterBaseAnimInstance->PlayAttackAnimation();
	}
}

void ALLL_MonsterBase::Stun()
{
	// Todo: 스턴 애니메이션 재생
}
