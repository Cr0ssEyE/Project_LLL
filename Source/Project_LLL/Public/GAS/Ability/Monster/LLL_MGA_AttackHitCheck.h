// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_AttackHitCheck.generated.h"

class ALLL_TA_TraceBase;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_AttackHitCheck : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
	void OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<ALLL_TA_TraceBase> TargetActorClass;
};
