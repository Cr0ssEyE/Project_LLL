// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LLL_ObjectGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ObjectGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULLL_ObjectGameplayAbilityBase();

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnCompleteCallBack();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnInterruptedCallBack();
};
