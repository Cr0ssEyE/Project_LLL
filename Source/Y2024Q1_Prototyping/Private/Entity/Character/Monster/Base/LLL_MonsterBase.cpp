// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/LLL_MonsterBase.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Monster/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/LLL_MonsterBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	// 몬스터 데이터 에셋 할당
	static ConstructorHelpers::FObjectFinder<ULLL_MonsterBaseDataAsset> DA_Monster(TEXT("/Script/Y2024Q1_Prototyping.LLL_MonsterBaseDataAsset'/Game/DataAsset/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}

	if (IsValid(MonsterDataAsset))
	{
		GetCapsuleComponent()->SetCapsuleSize(MonsterDataAsset->MonsterCollisionSize.Y, MonsterDataAsset->MonsterCollisionSize.X);
		GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
		
		GetMesh()->SetSkeletalMesh(MonsterDataAsset->CharacterBaseMesh);
		GetMesh()->SetAnimInstanceClass(MonsterDataAsset->CharacterAnimBlueprint);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, -MonsterDataAsset->MonsterCollisionSize.X));

		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed = MonsterDataAsset->MonsterBaseMoveSpeed;
		GetCharacterMovement()->MaxAcceleration = AccelerateSpeed = MonsterDataAsset->MonsterBaseAccelerateSpeed;
		GetCharacterMovement()->GroundFriction = GroundFriction = MonsterDataAsset->MonsterBaseGroundFriction;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, MonsterDataAsset->MonsterBaseTurnSpeed * 360.f, 0.f);
		GetCharacterMovement()->FallingLateralFriction = 3.0f;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_MonsterBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ALLL_MonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (MonsterBaseAnimInstance)
	{
		AttackAnimationEndHandle = MonsterBaseAnimInstance->AttackAnimationEndDelegate.AddUObject(this, &ALLL_MonsterBase::AttackAnimationEnd);
	}
}

float ALLL_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CharacterHealthAmount <= 0)
	{
		Dead();
	}
	else
	{
		Stun();
	}
	
	return 0;
}

void ALLL_MonsterBase::BeginDestroy()
{
	Super::BeginDestroy();

	MonsterBaseAnimInstance->AttackAnimationEndDelegate.Remove(AttackAnimationEndHandle);
}

void ALLL_MonsterBase::Stun()
{
	// Todo: 스턴 애니메이션 재생
	
	bStunAnimationIsPlaying = true;
}

void ALLL_MonsterBase::Attack()
{
	MonsterBaseAnimInstance->PlayAttackAnimation();

	bAttackAnimationIsPlaying = true;
}

void ALLL_MonsterBase::DamageToPlayer()
{
	// Todo: 플레이어에게 데미지 구현
}

void ALLL_MonsterBase::AttackAnimationEnd()
{
	bAttackAnimationIsPlaying = false;
}
