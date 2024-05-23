// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Object/Thrown/LLL_PlayerChaseHandAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Components/SphereComponent.h"
#include "Entity/Object/Thrown/LLL_PlayerChaseHand.h"

ULLL_PlayerChaseHandAttributeSet::ULLL_PlayerChaseHandAttributeSet() :
	CorrectionReachStateDistance(100.f)
{
	
}

bool ULLL_PlayerChaseHandAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if(Data.EvaluatedData.Magnitude < 0.f)
	{
		Data.EvaluatedData.Magnitude = 0.f;
	}
	return Super::PreGameplayEffectExecute(Data);
}

void ULLL_PlayerChaseHandAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
}
