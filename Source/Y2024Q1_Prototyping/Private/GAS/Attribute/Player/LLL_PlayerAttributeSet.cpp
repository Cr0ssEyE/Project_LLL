// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"

ULLL_PlayerAttributeSet::ULLL_PlayerAttributeSet()
{
	
}

void ULLL_PlayerAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwningActor());

	if (IsValid(Player))
	{
		Player->GetPlayerUIManager()->UpdateStatusWidget(GetMaxHealth(), GetCurrentHealth(), GetMaxShield(), GetCurrentShield());
	}
}
