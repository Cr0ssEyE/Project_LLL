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

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	virtual void KnockBackTarget();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "트리거 발생시 요구 태그")
	FGameplayTag TriggerRequiredTag;
	
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "넉백 발생시 적용 GE")
	TSubclassOf<UGameplayEffect> KnockBackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName ="커브 테이블 내 액션 넉백 배율 변화값 행")
	FCurveTableRowHandle KnockBackAmplifyChangeSection;
	
};
