// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LLL_MonsterGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_MonsterGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnCompleteCallBack();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnInterruptedCallBack();
};
