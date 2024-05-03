// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_KnockBack.generated.h"

class ALLL_TA_TraceBase;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_KnockBack : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_KnockBack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnTraceEndCallBack();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "넉백 발생시 적용 GE")
	TSubclassOf<UGameplayEffect> KnockBackEffect;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "넉백 발생시 넉백 거리 배율")
	float KnockBackMultiplier;
	
};
