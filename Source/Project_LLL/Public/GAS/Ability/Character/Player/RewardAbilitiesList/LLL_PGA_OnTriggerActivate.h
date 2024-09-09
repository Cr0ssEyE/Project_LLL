// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_PGA_RewardAbilityBase.h"
#include "LLL_PGA_OnTriggerActivate.generated.h"

#define CHASE_FEATHER_THROW_ANGLE_OFFSET 20.0f

class ALLL_ThrownObject;
class ALLL_AbilityObject;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_OnTriggerActivate : public ULLL_PGA_RewardAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_OnTriggerActivate();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void ApplyEffectWhenHit();
	virtual void SpawnAbilityObject();
	virtual void SpawnThrownObject();
	virtual void GrantTagWhenHit();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "트리거 발생시 요구 태그")
	FGameplayTagContainer TriggerRequiredTags;
	
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "액터 상태 요구 태그")
	FGameplayTag ActorStateRequiredTag;
	
	// GE
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적중시 GE 발생 여부")
	uint8 bUseOnAttackHitEffect : 1;
	
	UPROPERTY(EditAnywhere, Category = "GAS|ApplyEffect", DisplayName = "적중시 발생하는 GE", meta =(EditCondition = "bUseOnAttackHitEffect == true", EditConditionHides))
	TSubclassOf<UGameplayEffect> OnAttackHitEffect;

	// Object
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "어빌리티 오브젝트 생성 여부")
	uint8 bUseSpawnAbilityObject : 1;

	UPROPERTY(EditAnywhere, Category = "GAS|SpawnObject", DisplayName = "생성하는 오브젝트 클래스", meta =(EditCondition = "bUseSpawnAbilityObject == true", EditConditionHides))
	TSubclassOf<ALLL_AbilityObject> AbilityObjectClass;

	UPROPERTY(EditAnywhere, Category = "GAS|SpawnObject", DisplayName = "생성하는 오브젝트 기준 위치", meta =(EditCondition = "bUseSpawnAbilityObject == true", EditConditionHides))
	EEffectApplyTarget AbilityObjectLocationTarget;
	
	UPROPERTY(EditAnywhere, Category = "GAS|SpawnObject", DisplayName = "상대 위치", meta =(EditCondition = "bUseSpawnAbilityObject == true", EditConditionHides))
	FVector AbilityObjectOffsetLocation;;
	
	// Thrown
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "투사체 생성 여부")
	uint8 bUseSpawnThrownObject : 1;

	UPROPERTY(EditAnywhere, Category = "GAS|ThrownObject", DisplayName = "생성하는 투사체 클래스", meta =(EditCondition = "bUseSpawnThrownObject == true", EditConditionHides))
	TSubclassOf<ALLL_ThrownObject> ThrownObjectClass;

	UPROPERTY(EditAnywhere, Category = "GAS|ThrownObject", DisplayName = "투사체 속도", meta =(EditCondition = "bUseSpawnThrownObject == true", EditConditionHides))
	float ThrowSpeed;
	
	UPROPERTY(EditAnywhere, Category = "GAS|ThrownObject", DisplayName = "투사체 스폰 간격 시간", meta =(EditCondition = "bUseSpawnThrownObject == true", EditConditionHides))
	float SpawnOffsetTime;

	// Tag
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적중시 태그 부여 여부")
	uint8 bUseOnAttackHitGrantTag : 1;

	UPROPERTY(EditAnywhere, Category = "GAS|GrantTag", DisplayName = "태그 부여 목록", meta =(EditCondition = "bUseOnAttackHitGrantTag == true", EditConditionHides))
	FGameplayTagContainer GrantTagContainer;

	UPROPERTY(EditAnywhere, Category = "GAS|GrantTag", DisplayName = "태그 부여 갯수를 설정하는 어빌리티 데이터 값 태그", meta =(EditCondition = "bUseOnAttackHitGrantTag == true", EditConditionHides))
	FGameplayTag TagGrantNumTag;
	
	UPROPERTY(EditAnywhere, Category = "GAS|GrantTag", DisplayName = "태그 증가(true)|감소(false)", meta =(EditCondition = "bUseOnAttackHitGrantTag == true", EditConditionHides))
	uint8 bAdditiveOrSubtract : 1;
};
