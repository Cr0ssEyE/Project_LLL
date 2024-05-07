// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHandAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Components/SphereComponent.h"
#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"

ULLL_PlayerChaseHandAttributeSet::ULLL_PlayerChaseHandAttributeSet()
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

	if(Data.EvaluatedData.Attribute == GetCollisionRadiusAttribute())
	{
		const ALLL_PlayerChaseHand* ChaseHand = CastChecked<ALLL_PlayerChaseHand>(GetOwningAbilitySystemComponentChecked()->GetAvatarActor());
		SetCollisionRadius(Data.EvaluatedData.Magnitude);
		ChaseHand->GetCollisionComponent()->SetSphereRadius(GetCollisionRadius());
	}
}
