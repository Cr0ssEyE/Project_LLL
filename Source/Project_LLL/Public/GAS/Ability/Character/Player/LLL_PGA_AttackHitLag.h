// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_AttackHitLag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_AttackHitLag : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_AttackHitLag();
	
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "트리거 발생시 요구 태그")
	FGameplayTag TriggerRequiredTag;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "히트랙 발생 애님 몽타주")
	TObjectPtr<UAnimMontage> HitLagTargetMontage;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "테이블 데이터 사용 여부")
	uint8 bUseCurveDataTable : 1;
	
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "테이블 내 모션 별 히트랙 지속 시간 행", meta = (EditCondition = "bUseCurveDataTable == true", EditConditionHides))
	FCurveTableRowHandle HitLagDurationByMotion;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "테이블 내 모션 별 히트랙 감속 배율 행", meta = (EditCondition = "bUseCurveDataTable == true", EditConditionHides))
	FCurveTableRowHandle HitLagRateByMotion;
	
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "히트랙 지속 시간", meta = (EditCondition = "bUseCurveDataTable == false", EditConditionHides))
	float HitLagDuration;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "히트랙 감속 배율", meta = (EditCondition = "bUseCurveDataTable == false", EditConditionHides))
	float HitLagRate;
};
