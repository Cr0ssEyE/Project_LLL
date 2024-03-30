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
	// 플레이어
public:
	static float CalculateSkillGaugeIncrement(const float BaseValue, const float ComboAmplify, const float ItemAmplify)
	{
		const float CalculateResult = BaseValue * ComboAmplify * ItemAmplify;
		return CalculateResult;
	}
private:
	
};