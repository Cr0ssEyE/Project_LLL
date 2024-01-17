// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_ThrownObject.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLLConstructorHelper.h"

ALLL_RangedMonster::ALLL_RangedMonster()
{
	// 몬스터 데이터 에셋 할당
	RangedMonsterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_RangedMonsterDataAsset>(PATH_RANGED_MONSTER_DATA, EAssertionLevel::Check);
	if (IsValid(RangedMonsterDataAsset))
	{
		GetCapsuleComponent()->SetCapsuleSize(RangedMonsterDataAsset->CollisionSize.Y, RangedMonsterDataAsset->CollisionSize.X);
		
		GetMesh()->SetSkeletalMesh(RangedMonsterDataAsset->SkeletalMesh);
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, -RangedMonsterDataAsset->CollisionSize.X));

		UClass* AnimBlueprint = RangedMonsterDataAsset->AnimInstance.LoadSynchronous();
		if (IsValid(AnimBlueprint))
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprint);
		}

		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed = RangedMonsterDataAsset->MoveSpeed;
		GetCharacterMovement()->MaxAcceleration = AccelerateSpeed = RangedMonsterDataAsset->AccelerateSpeed;
		GetCharacterMovement()->GroundFriction = GroundFriction = RangedMonsterDataAsset->GroundFriction;
		GetCharacterMovement()->RotationRate = FRotator(0.f, RangedMonsterDataAsset->TurnSpeed * 360.f, 0.f);

		MaxHealthAmount = RangedMonsterDataAsset->Health;
		CurrentHealthAmount = MaxHealthAmount;
		MaxShieldAmount = RangedMonsterDataAsset->ShieldAmount;
		CurrentShieldAmount = MaxShieldAmount;
		OffensePower = RangedMonsterDataAsset->OffensePower;
		AttackDistance = RangedMonsterDataAsset->AttackDistance;

		DetectDistance = RangedMonsterDataAsset->DetectDistance;
		FieldOfView = RangedMonsterDataAsset->FieldOfView;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_RangedMonsterAIController::StaticClass();
}

bool ALLL_RangedMonster::AttackAnimationIsPlaying()
{
	Super::AttackAnimationIsPlaying();

	const ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(MonsterBaseAnimInstance))
	{
		return MonsterBaseAnimInstance->Montage_IsPlaying(RangedMonsterDataAsset->AttackAnimMontage);
	}

	return false;
}

void ALLL_RangedMonster::ThrowToPlayer()
{
	ALLL_ThrownObject* ThrownObject = GetWorld()->SpawnActor<ALLL_ThrownObject>();
	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(ThrownObject) && IsValid(PlayerBase))
	{
		const float Speed = ThrownObject->GetSpeed();
		const float Distance = GetDistanceTo(PlayerBase);
		const FVector StartLocation = GetActorLocation();
		const FVector PredictedMove = PlayerBase->GetVelocity() * Distance / Speed;
		const FVector PredictedLocation = PlayerBase->GetActorLocation() + PredictedMove * RangedMonsterDataAsset->PredictionRate;
		const FVector PredictedDirection = (PredictedLocation - StartLocation).GetSafeNormal();
		
		const FRotator PredictedRotation = FRotationMatrix::MakeFromX(PredictedDirection).Rotator();
	
		ThrownObject->SetActorLocationAndRotation(StartLocation, PredictedRotation);
		ThrownObject->Throw(this);
	}
}
