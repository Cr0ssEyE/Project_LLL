// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_ThrownObject.h"
#include "Util/LLLConstructorHelper.h"

ALLL_RangedMonster::ALLL_RangedMonster()
{
	// 몬스터 데이터 에셋 할당
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_RangedMonsterDataAsset>(PATH_RANGED_MONSTER_DATA, EAssertionLevel::Check);
	RangedMonsterDataAsset = Cast<ULLL_RangedMonsterDataAsset>(CharacterDataAsset);
	if (IsValid(RangedMonsterDataAsset))
	{
		DetectDistance = RangedMonsterDataAsset->DetectDistance;
		FieldOfView = RangedMonsterDataAsset->FieldOfView;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_RangedMonsterAIController::StaticClass();
}

void ALLL_RangedMonster::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(CharacterAnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance())))
	{
		CharacterAnimInstance->SetDataAsset(CharacterDataAsset);
	}
}

bool ALLL_RangedMonster::AttackAnimationIsPlaying()
{
	Super::AttackAnimationIsPlaying();

	if (IsValid(CharacterAnimInstance))
	{
		return CharacterAnimInstance->Montage_IsPlaying(CharacterDataAsset->AttackAnimMontage);
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
