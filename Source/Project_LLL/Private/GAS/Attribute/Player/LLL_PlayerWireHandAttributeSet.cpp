// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PlayerWireHandAttributeSet::ULLL_PlayerWireHandAttributeSet()
{
	
}

bool ULLL_PlayerWireHandAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void ULLL_PlayerWireHandAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
}
