// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

class PROJECT_LLL_API FLLL_ExecuteCueHelper
{
public:
	static void ExecuteCue(const UObject* NewSourceObject, UAbilitySystemComponent* ASC, FGameplayTag CueTag)
	{
		FGameplayEffectContextHandle CueContextHandle = ASC->MakeEffectContext();
		CueContextHandle.AddSourceObject(NewSourceObject);
		FGameplayCueParameters CueParam;
		CueParam.EffectContext = CueContextHandle;

		ASC->ExecuteGameplayCue(CueTag, CueParam);
	}
};
