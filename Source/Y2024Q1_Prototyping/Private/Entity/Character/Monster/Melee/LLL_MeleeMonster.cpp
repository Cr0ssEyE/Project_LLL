// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonsterAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MeleeMonster::ALLL_MeleeMonster()
{
	// 몬스터 데이터 에셋 할당
	MeleeMonsterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MeleeMonsterDataAsset>(PATH_MELEE_MONSTER_DATA, EAssertionLevel::Check);
	if (IsValid(MeleeMonsterDataAsset))
	{
		GetCapsuleComponent()->SetCapsuleSize(MeleeMonsterDataAsset->CollisionSize.Y, MeleeMonsterDataAsset->CollisionSize.X);
		
		GetMesh()->SetSkeletalMesh(MeleeMonsterDataAsset->SkeletalMesh);
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, -MeleeMonsterDataAsset->CollisionSize.X));

		UClass* AnimBlueprint = MeleeMonsterDataAsset->AnimInstance.LoadSynchronous();
		if (IsValid(AnimBlueprint))
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprint);
		}

		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed = MeleeMonsterDataAsset->MoveSpeed;
		GetCharacterMovement()->MaxAcceleration = AccelerateSpeed = MeleeMonsterDataAsset->AccelerateSpeed;
		GetCharacterMovement()->GroundFriction = GroundFriction = MeleeMonsterDataAsset->GroundFriction;
		GetCharacterMovement()->RotationRate = FRotator(0.f, MeleeMonsterDataAsset->TurnSpeed * 360.f, 0.f);

		MaxHealthAmount = MeleeMonsterDataAsset->Health;
		CurrentHealthAmount = MaxHealthAmount;
		MaxShieldAmount = MeleeMonsterDataAsset->ShieldAmount;
		CurrentShieldAmount = MaxShieldAmount;
		OffensePower = MeleeMonsterDataAsset->OffensePower;
		AttackDistance = MeleeMonsterDataAsset->AttackDistance;

		DetectDistance = MeleeMonsterDataAsset->DetectDistance;
		FieldOfView = MeleeMonsterDataAsset->FieldOfView;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_MeleeMonsterAIController::StaticClass();
}

bool ALLL_MeleeMonster::AttackAnimationIsPlaying()
{
	Super::AttackAnimationIsPlaying();

	const ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(MonsterBaseAnimInstance))
	{
		return MonsterBaseAnimInstance->Montage_IsPlaying(MeleeMonsterDataAsset->AttackAnimMontage);
	}

	return false;
}

void ALLL_MeleeMonster::DamageToPlayer()
{
	// Todo: 플레이어에게 데미지 구현
}
