// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Monster/Base/LLL_MonsterAttributeSet.h"

void ULLL_MonsterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		OnLoadMaxHealthDelegate.Broadcast(GetMaxHealth());
	}
}
