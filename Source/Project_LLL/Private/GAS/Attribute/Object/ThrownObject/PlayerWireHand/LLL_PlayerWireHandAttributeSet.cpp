// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Object/ThrownObject/PlayerWireHand/LLL_PlayerWireHandAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Components/SphereComponent.h"
#include "Entity/Object/Thrown/PlayerWireHand/LLL_PlayerWireHand.h"

ULLL_PlayerWireHandAttributeSet::ULLL_PlayerWireHandAttributeSet()
{
	
}

bool ULLL_PlayerWireHandAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if(Data.EvaluatedData.Magnitude < 0.f)
	{
		Data.EvaluatedData.Magnitude = 0.f;
	}
	return Super::PreGameplayEffectExecute(Data);
}

void ULLL_PlayerWireHandAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetCollisionRadiusAttribute())
	{
		const ALLL_PlayerWireHand* WireHand = CastChecked<ALLL_PlayerWireHand>(GetOwningAbilitySystemComponentChecked()->GetAvatarActor());
		SetCollisionRadius(Data.EvaluatedData.Magnitude);
		WireHand->GetCollisionComponent()->SetSphereRadius(GetCollisionRadius());
	}
}
