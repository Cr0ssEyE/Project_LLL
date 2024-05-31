// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_AttackHitCheck.generated.h"

class UFMODEvent;
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
	void OnTraceEndCallBack(FGameplayEventData EventData);
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "공격 적중시 적중 갯수에 영향을 받는 이펙트(자신 적용)")
	TArray<TSubclassOf<UGameplayEffect>> CheckHitCountEffects;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> AttackDamageEffect;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> GiveTagEffect;

	UPROPERTY(EditAnywhere, DisplayName = "공격 충돌 FMod 이벤트 파라미터 값")
	float PlayerAttackHitCountParameterValue;

protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<ALLL_TA_TraceBase> TargetActorClass;
	
	UPROPERTY()
	TObjectPtr<ULLL_AT_Trace> TraceTask;
};
