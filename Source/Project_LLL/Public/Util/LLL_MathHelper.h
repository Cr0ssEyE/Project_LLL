// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class PROJECT_LLL_API FLLL_MathHelper
{
public:
	static uint32 CalculatePlayerWeaponDamage(uint32 CharacterOffensePower, uint32 WeaponOffensePower, float WeaponActionOffenseMultiply)
	{
		const uint32 CalculateResult = (CharacterOffensePower + WeaponOffensePower) * WeaponActionOffenseMultiply;
		return CalculateResult;
	}

	static FVector CalculateComponentFrontPoint(USceneComponent* Component, float Multiply)
	{
		const FVector CalculateResult = Component->GetComponentLocation() + Multiply * Component->GetForwardVector();
		return CalculateResult;
	}
private:
	
};