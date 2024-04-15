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

	static FVector CalculateLaunchVelocity(const FVector Direction, const float Multiply)
	{
		const FVector CalculateResult = Direction * Multiply * 10.f;
		return CalculateResult;
	}

	static FVector GetPredictedLocation(const APawn* Owner, const APawn* Target, float TargetSpeed, float PredictionRate)
	{
		const float Distance = Owner->GetDistanceTo(Target);
		const FVector PredictedMove = Target->GetVelocity() * (Distance / TargetSpeed);
		const FVector PredictedLocation = Target->GetActorLocation() + PredictedMove * PredictionRate;
		return PredictedLocation;
	}

	static bool IsInFieldOfView(const APawn* Owner, const APawn* Target, float FieldOfView)
	{
		FVector DirectionToTarget = Target->GetActorLocation() - Owner->GetActorLocation();
		DirectionToTarget.Normalize();

		const FVector OwnerForwardVector = Owner->GetActorForwardVector();
		const float DotProduct = FVector::DotProduct(OwnerForwardVector, DirectionToTarget);

		if (FMath::Acos(DotProduct) <= FMath::DegreesToRadians(FieldOfView / 2.0f))
		{
			return true;
		}

		return false;
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