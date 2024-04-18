// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_AttackHitCheck.generated.h"

class ULLL_AT_Trace;
class ULLL_PGA_KnockBack;
class ALLL_TA_TraceBase;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_AttackHitCheck : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_AttackHitCheck();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnTraceEndCallBack();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> ComboStackEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> IncreaseSkillGaugeEffect;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> GiveTagEffect;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<ALLL_TA_TraceBase> TargetActorClass;

	UPROPERTY()
	TObjectPtr<ULLL_AT_Trace> TraceTask;
	
	float CurrentLevel = 0;
	
};
