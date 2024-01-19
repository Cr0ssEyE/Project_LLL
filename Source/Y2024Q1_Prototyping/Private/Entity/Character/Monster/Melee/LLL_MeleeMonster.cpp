// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"

#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonsterAIController.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MeleeMonster::ALLL_MeleeMonster()
{
	// 몬스터 데이터 에셋 할당
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MeleeMonsterDataAsset>(PATH_MELEE_MONSTER_DATA, EAssertionLevel::Check);
	MeleeMonsterDataAsset = Cast<ULLL_MeleeMonsterDataAsset>(CharacterDataAsset);
	if (IsValid(MeleeMonsterDataAsset))
	{
		DetectDistance = MeleeMonsterDataAsset->DetectDistance;
		FieldOfView = MeleeMonsterDataAsset->FieldOfView;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_MeleeMonsterAIController::StaticClass();
}

void ALLL_MeleeMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(CharacterAnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance())))
	{
		CharacterAnimInstance->SetDataAsset(CharacterDataAsset);
	}
}

bool ALLL_MeleeMonster::AttackAnimationIsPlaying()
{
	Super::AttackAnimationIsPlaying();

	if (IsValid(CharacterAnimInstance))
	{
		return CharacterAnimInstance->Montage_IsPlaying(CharacterDataAsset->AttackAnimMontage);
	}

	return false;
}

void ALLL_MeleeMonster::DamageToPlayer()
{
	float AttackRadius = 50.0f;
	
	FHitResult HitResult;
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * AttackDistance;
	const FQuat Rot = FQuat::Identity;
	FCollisionShape Shape = FCollisionShape::MakeSphere(AttackRadius);
	const bool bSweepResult = GetWorld()->SweepSingleByChannel(HitResult, Start, End, Rot, ECC_PLAYER_ONLY, Shape);
	
	if (bSweepResult)
	{
		if (AActor* TargetActor = HitResult.GetActor())
		{
			float AttackDamage = CharacterDataAsset->OffensePower;
			FDamageEvent DamageEvent;
			TargetActor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
	}

	const FVector Center = Start + (End - Start) * 0.5f;
	const float HalfHeight = AttackDistance * 0.5f;
	const FQuat CapsuleRotate = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
	const FColor DrawColor = bSweepResult ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRotate, DrawColor, false, 1.f);
}
