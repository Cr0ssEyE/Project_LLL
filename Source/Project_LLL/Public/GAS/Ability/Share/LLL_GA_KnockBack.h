// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Base/LLL_CharacterGameplayAbilityBase.h"
#include "LLL_GA_KnockBack.generated.h"

class ALLL_TA_TraceBase;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GA_KnockBack : public ULLL_CharacterGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_GA_KnockBack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION()
	void OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "타겟 식별용 TA")
	TSubclassOf<ALLL_TA_TraceBase> TargetActorClass;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "넉백 발생시 적용 GE")
	TSubclassOf<UGameplayEffect> KnockBackEffect;
	
};
