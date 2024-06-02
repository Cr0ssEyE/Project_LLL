// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_PGA_RewardAbilityBase.h"
#include "LLL_PGA_OnAttackHit.generated.h"

class ALLL_ThrownObject;
class ALLL_AbilityObject;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_OnAttackHit : public ULLL_PGA_RewardAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_OnAttackHit();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void ApplyEffectWhenHit();

	virtual void SpawnObjectWhenHit();

	virtual void SpawnThrownWhenHit();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "트리거 발생시 요구 태그")
	FGameplayTag TriggerRequiredTag;

	// GE
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적중시 GE 발생 여부")
	uint8 bUseOnAttackHitEffect : 1;
	
	UPROPERTY(EditAnywhere, Category = "GAS|ApplyEffect", DisplayName = "적중시 발생하는 GE", meta =(EditCondition = "bUseOnAttackHitEffect == true", EditConditionHides))
	TSubclassOf<UGameplayEffect> OnAttackHitEffect;

	// Object
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적중시 오브젝트 생성 여부")
	uint8 bUseOnAttackHitSpawnObject : 1;

	UPROPERTY(EditAnywhere, Category = "GAS|SpawnObject", DisplayName = "적중시 생성하는 오브젝트 클래스", meta =(EditCondition = "bUseOnAttackHitSpawnObject == true", EditConditionHides))
	TSubclassOf<ALLL_AbilityObject> AbilityObjectClass;

	UPROPERTY(EditAnywhere, Category = "GAS|SpawnObject", DisplayName = "적중시 생성하는 오브젝트 기준 위치", meta =(EditCondition = "bUseOnAttackHitSpawnObject == true", EditConditionHides))
	EEffectApplyTarget AbilityObjectLocationTarget;
	
	// Thrown
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적중시 투사체 생성 여부")
	uint8 bUseOnAttackHitSpawnThrown : 1;

	UPROPERTY(EditAnywhere, Category = "GAS|ThrownObject", DisplayName = "적중시 생성하는 투사체 클래스", meta =(EditCondition = "bUseOnAttackHitSpawnThrown == true", EditConditionHides))
	TSubclassOf<ALLL_ThrownObject> ThrownObjectClass;

	UPROPERTY(EditAnywhere, Category = "GAS|ThrownObject", DisplayName = "투사체 속도", meta =(EditCondition = "bUseOnAttackHitSpawnThrown == true", EditConditionHides))
	float ThrowSpeed;
	
	UPROPERTY(EditAnywhere, Category = "GAS|ThrownObject", DisplayName = "투사체 스폰 간격 시간", meta =(EditCondition = "bUseOnAttackHitSpawnThrown == true", EditConditionHides))
	float SpawnOffsetTime;
};
