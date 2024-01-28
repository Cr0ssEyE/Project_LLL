// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonsterAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_ThrownObject.h"
#include "Game/ProtoGameInstance.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
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

	ObjectPooling = CreateDefaultSubobject<ULLL_ObjectPoolingComponent>(TEXT("ObjectPooling"));
}

void ALLL_RangedMonster::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(CharacterAnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance())))
	{
		CharacterAnimInstance->SetDataAsset(CharacterDataAsset);
	}
}

void ALLL_RangedMonster::ThrowToPlayer()
{
	ALLL_ThrownObject* ThrownObject = Cast<ALLL_ThrownObject>(ObjectPooling->GetActor(ALLL_ThrownObject::StaticClass()));
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

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterAttackDebug())
			{
				DrawDebugLine(GetWorld(), StartLocation, PredictedLocation, FColor::Red, false, 1.f);
			}
		}
#endif
	}
}
