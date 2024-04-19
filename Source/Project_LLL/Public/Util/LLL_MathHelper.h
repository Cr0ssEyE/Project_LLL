// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class PROJECT_LLL_API FLLL_MathHelper
{
	// 범용
public:
	static FVector CalculateComponentFrontPoint(const USceneComponent* Component, const float Multiply)
	{
		const FVector CalculateResult = Component->GetComponentLocation() + Multiply * Component->GetForwardVector();
		return CalculateResult;
	}

	static FVector CalculateLaunchVelocity(const FVector& Direction, const float Multiply)
	{
		const FVector CalculateResult = Direction * Multiply * 10.f;
		return CalculateResult;
	}

	static FVector GetPredictedLocation(const AActor* Owner, const AActor* Target, float TargetSpeed, float PredictionRate)
	{
		const float Distance = Owner->GetDistanceTo(Target);
		const FVector PredictedMove = Target->GetVelocity() * (Distance / TargetSpeed);
		const FVector PredictedLocation = Target->GetActorLocation() + PredictedMove * PredictionRate;
		return PredictedLocation;
	}

	static bool IsInFieldOfView(const AActor* Owner, const AActor* Target, float Distance, float FieldOfView)
	{
		if (Distance < Owner->GetDistanceTo(Target))
		{
			return false;
		}
		
		FVector DirectionToTarget = Target->GetActorLocation() - Owner->GetActorLocation();
		DirectionToTarget.Normalize();

		const FVector OwnerForwardVector = Owner->GetActorForwardVector();
		const float DotProduct = FVector::DotProduct(OwnerForwardVector, DirectionToTarget);

		if (FMath::Acos(DotProduct) > FMath::DegreesToRadians(FieldOfView / 2.0f))
		{
			return false;
		}

		return true;
	}

	static FVector CalcualteLaunchablePosition(const UWorld& World, const AActor* Owner, const float LaunchDistance, const FVector& LaunchDirection, const float ArrivalDistance, const FName CollisionProfile)
	{
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);

		FVector LaunchablePosition = Owner->GetActorLocation() + LaunchDirection.GetSafeNormal() * LaunchDistance;
		World.LineTraceSingleByProfile(
			HitResult,
			Owner->GetActorLocation(),
			LaunchablePosition,
			CollisionProfile,
			Params
			);

		if (!HitResult.GetActor())
		{
			return LaunchablePosition;
		}

		// 충돌한 대상이 있는 경우 = 해당 충돌 위치에서 도달 인식이 되는 위치를 목표 위치로 설정
		FVector MovablePosition = HitResult.ImpactPoint - LaunchDirection * ArrivalDistance;
		return MovablePosition;
	}

	static bool CheckLaunchablePosition(const UWorld* World, const AActor* Owner, const float LaunchDistance, const FVector& LaunchDirection, const FName CollisionProfile)
	{
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
	
	// 플레이어
public:
	static float CalculateSkillGaugeIncrement(const float BaseValue, const float ComboAmplify, const float ItemAmplify)
	{
		const float CalculateResult = BaseValue * ComboAmplify * ItemAmplify;
		return CalculateResult;
	}
private:
	
};