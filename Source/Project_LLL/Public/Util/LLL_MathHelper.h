﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/Character.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"

class PROJECT_LLL_API FLLL_MathHelper
{
	// 범용
public:
	static FVector CalculateComponentFrontPoint(const USceneComponent* Component, const float Multiply)
	{
		if (!IsValid(Component))
		{
			return FVector::Zero();
		}
		
		const FVector CalculateResult = Component->GetComponentLocation() + Multiply * Component->GetForwardVector();
		return CalculateResult;
	}

	static FVector CalculateLaunchVelocity(const FVector& Direction, const float Multiply)
	{
		const FVector CalculateResult = Direction.GetSafeNormal2D() * Multiply * 10.f;
		return CalculateResult;
	}

	static FVector GetPredictedLocation(const AActor* ThrownActor, const AActor* Target, const FVector& Velocity, float TargetSpeed, float PredictionRate)
	{
		if (!IsValid(ThrownActor) || !IsValid(Target))
		{
			return FVector::Zero();
		}
		
		const float Distance = ThrownActor->GetDistanceTo(Target);
		const FVector PredictedMove = Velocity * (Distance / TargetSpeed);
		const FVector PredictedLocation = Target->GetActorLocation() + PredictedMove * PredictionRate;
		return PredictedLocation;
	}

	static FVector CalculatePredictedDirection(const AActor* Thrower, FVector PredictedDirection)
	{
		const FVector ThrowerForward = Thrower->GetActorForwardVector();
		const float Cos = FMath::Cos(FMath::DegreesToRadians(45.0f));
		if (FVector::DotProduct(ThrowerForward, PredictedDirection) < Cos)
		{
			const FVector RightVector = FVector::CrossProduct(ThrowerForward, FVector::UpVector);
			if (FVector::DotProduct(RightVector, PredictedDirection) > 0.0f)
			{
				PredictedDirection = FRotationMatrix::MakeFromX(ThrowerForward).TransformVector(FVector(Cos, FMath::Sqrt(1 - FMath::Square(Cos)) * -1.0f, 0));
			}
			else
			{
				PredictedDirection = FRotationMatrix::MakeFromX(ThrowerForward).TransformVector(FVector(Cos, FMath::Sqrt(1 - FMath::Square(Cos)), 0));
			}
		}

		return PredictedDirection;
	}

	static bool IsInFieldOfView(const AActor* Owner, const AActor* Target, float Distance, float FieldOfView, const FRotator& Rotation = FRotator::ZeroRotator)
	{
		if (Distance < Owner->GetDistanceTo(Target))
		{
			return false;
		}
		
		FVector DirectionToTarget = Target->GetActorLocation() - Owner->GetActorLocation();
		DirectionToTarget.Normalize();

		const FVector ForwardVector = Rotation.RotateVector(Owner->GetActorForwardVector());
		const float DotProduct = FVector::DotProduct(ForwardVector, DirectionToTarget);

		if (FMath::Acos(DotProduct) > FMath::DegreesToRadians(FieldOfView / 2.0f))
		{
			return false;
		}

		return true;
	}
	
	static bool CheckLaunchablePosition(const UWorld* World, const ACharacter* Owner, const float LaunchDistance, const FVector& LaunchDirection, const FName CollisionProfile)
	{
		if (!IsValid(World) || !IsValid(Owner))
		{
			return false;
		}
		
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);

		FVector LaunchablePosition = Owner->GetActorLocation() + LaunchDirection.GetSafeNormal() * LaunchDistance;
		World->LineTraceSingleByProfile(
			HitResult,
			Owner->GetActorLocation(),
			LaunchablePosition,
			CollisionProfile,
			Params
			);
		
		return !HitResult.GetActor();
	}

	static bool CheckFallableKnockBackPower(const UWorld* World, float KnockBackPower)
	{
		if (!IsValid(World))
	{
			return false;
		}
		
		float FallableCheckPower = 100.f;
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
		if (!IsValid(Character))
		{
			return false;
		}
		
		if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Character))
		{
			const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(Player->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

			FallableCheckPower = PlayerAttributeSet->GetFalloutablePower();
		}
		else
		{
			ensure(false);
		}
		
		if (KnockBackPower >= FallableCheckPower)
		{
			return true;
		}
		
		return false;
	}
	
	// 플레이어
public:
	static float CalculatePlayerSkillGaugeIncrement(const float BaseValue, const float ComboAmplify, const float ItemAmplify)
	{
		const float CalculateResult = BaseValue * ComboAmplify * ItemAmplify;
		return CalculateResult;
	}

	static float CalculateCriticalDamage(const float OffensePower, const ALLL_PlayerBase* Player)
	{
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

		float CriticalChance = PlayerAttributeSet->GetCriticalChance();
		CriticalChance += PlayerAttributeSet->GetCriticalChancePlus();
	
		bool bIsChance = false;
		if (CriticalChance != 0.0f)
		{
			bIsChance = FMath::RandRange(0.0f, 1.0f) <= CriticalChance;
		}

		if (bIsChance)
		{
			UE_LOG(LogTemp, Log, TEXT("치명타 발동 (확률 : %.2f%%)"), CriticalChance * 100.0f)
		}
		
		return bIsChance ? OffensePower * PlayerAttributeSet->GetCriticalAmplify() : OffensePower;
	}
	
	static FVector CalculatePlayerLaunchableLocation(const UWorld* World, const ACharacter* Owner, const float LaunchDistance , const float CorrectionDistance, const FVector& LaunchDirection)
	{
		if (!IsValid(World) || !IsValid(Owner))
        {
        	return FVector::Zero();
        }
        		
		FHitResult CapsuleHitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		const FVector LaunchLocation = Owner->GetActorLocation() + LaunchDirection.GetSafeNormal2D() * LaunchDistance;
		const FVector2d CapsuleExtent = FVector2d(Owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.2f, Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
		
		// 끼임 방지용 캐릭터 충돌 캡슐보다 1.2배 큰 반지름 체크
		World->SweepSingleByChannel(
			CapsuleHitResult,
			LaunchLocation,
			LaunchLocation,
			FQuat::Identity,
			ECC_WALL_ONLY,
			FCollisionShape::MakeCapsule(CapsuleExtent.X, CapsuleExtent.Y),
			Params
			);

		// 해당 범위에 아무런 액터도 충돌하지 않음 = 이동 거리만큼 이동해도 문제 없음
		if (!CapsuleHitResult.GetActor())
		{
			// 바닥 체크
			World->SweepSingleByChannel(
			CapsuleHitResult,
			LaunchLocation,
			LaunchLocation,
			FQuat::Identity,
			ECC_WALL_ONLY,
			FCollisionShape::MakeCapsule(5.f, CapsuleExtent.Y * 10),
			Params
			);
			
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(World->GetGameInstance()))
			{
				if(DebugGameInstance->CheckPlayerDashDebug())
				{
					DrawDebugCapsule(World, LaunchLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Yellow, false, 2.f);
				}
			}
#endif
			if (Cast<UStaticMeshComponent>(CapsuleHitResult.GetComponent()))
			{
				return LaunchLocation;
			}
		}

		// 대쉬 보정 범위 체크
		for (float MultiplyValue = 10.f; MultiplyValue < CorrectionDistance; MultiplyValue += 10.f)
		{
			FHitResult CorrectionLocationHitResult;
			FVector CorrectionLaunchLocation = LaunchLocation + LaunchDirection.GetSafeNormal2D() * MultiplyValue;
			World->SweepSingleByChannel(
				CorrectionLocationHitResult,
				CorrectionLaunchLocation,
				CorrectionLaunchLocation,
				FQuat::Identity,
				ECC_WALL_ONLY,
				FCollisionShape::MakeCapsule(CapsuleExtent.X, CapsuleExtent.Y),
				Params
				);
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(World->GetGameInstance()))
			{
				if(DebugGameInstance->CheckPlayerDashDebug())
				{
					DrawDebugCapsule(World, CorrectionLaunchLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Magenta, false, 2.f);
				}
			}
#endif
			
			if (!CorrectionLocationHitResult.GetActor())
			{
				// 바닥 체크
				World->SweepSingleByChannel(
				CorrectionLocationHitResult,
				CorrectionLaunchLocation,
				CorrectionLaunchLocation,
				FQuat::Identity,
				ECC_WALL_ONLY,
				FCollisionShape::MakeCapsule(5.f, CapsuleExtent.Y * 10),
				Params
				);
				
				if (Cast<UStaticMeshComponent>(CorrectionLocationHitResult.GetComponent()))
				{
					return CorrectionLaunchLocation;
				}
			}
		}

		// 보정범위 제외 가장 멀리 이동할 수 있는 거리 체크
		for (float MultiplyValue = 10.f; MultiplyValue < LaunchDistance; MultiplyValue += 10.f)
		{
			FHitResult FurthestLocationHitResult;
			FVector NewLocation = LaunchLocation - LaunchDirection.GetSafeNormal2D() * MultiplyValue;
			World->SweepSingleByChannel(
				FurthestLocationHitResult,
				NewLocation,
				NewLocation,
				FQuat::Identity,
				ECC_WALL_ONLY,
				FCollisionShape::MakeCapsule(CapsuleExtent.X, CapsuleExtent.Y),
				Params
				);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(World->GetGameInstance()))
			{
				if(DebugGameInstance->CheckPlayerDashDebug())
				{
					DrawDebugCapsule(World, NewLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Cyan, false, 2.f);
				}
			}
#endif
			if (!FurthestLocationHitResult.GetActor())
			{
				// 바닥 체크
				World->SweepSingleByChannel(
				FurthestLocationHitResult,
				NewLocation,
				NewLocation,
				FQuat::Identity,
				ECC_WALL_ONLY,
				FCollisionShape::MakeCapsule(5.f, CapsuleExtent.Y * 10),
				Params
				);

				if (Cast<UStaticMeshComponent>(FurthestLocationHitResult.GetComponent()))
				{
					// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 장거리 체크 발동")));
					return NewLocation;
				}
			}
		}
		
		// 여긴 사실상 들어오면 안됨
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 거리 계산 오류 발생")));
		// DrawDebugLine(World, Owner->GetActorLocation(), LaunchLocation, FColor::Magenta, false, 2.f);
		// DrawDebugCapsule(World, LaunchLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Magenta, false, 2.f);
		return Owner->GetActorLocation();
	}

	static float CalculatePlayerKnockBackCollisionCheckEndApproximation(const float KnockBackPower)
	{
		// 1000.f 기준 약 0.1초 뒤에 속도가 0에 근접함.
		// 5000.f 기준 약 0.2초 뒤에 속도가 0에 근접함.
		// 40000.f 기준 약 0.3초 뒤에 속도가 0에 근접함. 이 이상 플레이어의 공격으로 넉백 당하지 않을거라 생각해 0.3초를 최대 값으로 지정
		// 40000.f로 얼마나 멀리 날아가는지 궁금하시다면 BP_VelocityTestActor에서 Velocity 벡터 값중 아무 한군데 40000.f를 넣어보세요.
		// 위 CalculateLaunchVelocity 함수를 보면 아시겠지만 입력값에 *10 처리 하고 있어서 받는 값은 실제 속도의 1/10.
		
		const float Result = 0.2f;
		const float MinTime = 0.1f;
		const float MaxTime = 0.3f;

		if (KnockBackPower - 500.f <= 0)
		{
			const float Alpha = KnockBackPower / 500.f;
			const float TimeSubtractValue = FMath::Lerp(0.1f, 0.f, Alpha);
			return FMath::Clamp(Result - TimeSubtractValue, MinTime, Result);
		}

		if (KnockBackPower - 500.f > 0)
		{
			const float Alpha = 500.f / KnockBackPower;
			const float TimeAdditiveValue = FMath::Lerp(0.1f, 0.f, Alpha);
			return FMath::Clamp(Result + TimeAdditiveValue, Result, MaxTime);
		}
		
		return Result;
	}
private:
	
};
