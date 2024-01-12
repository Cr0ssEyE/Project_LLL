// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/LLL_MonsterBase.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/LLL_MonsterBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	// 몬스터 데이터 에셋 할당
	MonsterBaseDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MonsterBaseDataAsset>(PATH_MONSTER_DATA, EAssertionLevel::Check);
	if (IsValid(MonsterBaseDataAsset))
	{
		GetCapsuleComponent()->SetCapsuleSize(MonsterBaseDataAsset->CollisionSize.Y, MonsterBaseDataAsset->CollisionSize.X);
		GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
		
		GetMesh()->SetSkeletalMesh(MonsterBaseDataAsset->SkeletalMesh);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, -MonsterBaseDataAsset->CollisionSize.X));

		UClass* AnimBlueprint = MonsterBaseDataAsset->AnimInstance.LoadSynchronous();
		if (IsValid(AnimBlueprint))
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprint);
		}

		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed = MonsterBaseDataAsset->MoveSpeed;
		GetCharacterMovement()->MaxAcceleration = AccelerateSpeed = MonsterBaseDataAsset->AccelerateSpeed;
		GetCharacterMovement()->GroundFriction = GroundFriction = MonsterBaseDataAsset->GroundFriction;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, MonsterBaseDataAsset->TurnSpeed * 360.f, 0.f);
		GetCharacterMovement()->FallingLateralFriction = 3.0f;

		Health = MonsterBaseDataAsset->Health;
		ShieldAmount = MonsterBaseDataAsset->ShieldAmount;
		OffensePower = MonsterBaseDataAsset->OffensePower;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_MonsterBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

bool ALLL_MonsterBase::AttackAnimationIsPlaying()
{
	return MonsterBaseAnimInstance->Montage_IsPlaying(MonsterBaseDataAsset->AttackAnimMontage);
}

void ALLL_MonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
}

float ALLL_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Health <= 0)
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

void ALLL_MonsterBase::Stun()
{
	// Todo: 스턴 애니메이션 재생
}

void ALLL_MonsterBase::Attack()
{
	MonsterBaseAnimInstance->PlayAttackAnimation();
}

void ALLL_MonsterBase::DamageToPlayer()
{
	// Todo: 플레이어에게 데미지 구현
}
