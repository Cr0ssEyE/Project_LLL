﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "LLL_ExtendedGameplayEffect.generated.h"

enum class EEffectApplyTarget : uint8;
struct FAbilityDataTable;
class ULLL_PGA_RewardAbilityBase;
enum class EEffectAccessRange : uint8;
enum class EEffectOwnerType : uint8;
enum class EAbilityCategory : uint8;
enum class EAbilityRank : uint8;
enum class EAbilityPart : uint8;
enum class EAbilityType : uint8;

/**
 * 원래는 그냥 UGameplayEffect를 사용하려고 했는데, 장기적으로 봤을 때 이펙트의 사용이나 적용 외적인 측면에서 설정이 어느정도 필요하다 판단하여 사용함.
 */
UCLASS()
class PROJECT_LLL_API ULLL_ExtendedGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	FORCEINLINE EEffectOwnerType GetOwnerShip() const { return EffectOwnership; }
	FORCEINLINE EEffectAccessRange GetAccessRange() const { return EffectAccessRange; }
	FORCEINLINE EEffectApplyTarget GetEffectApplyTarget() const { return EffectApplyTarget; }
	FORCEINLINE int32 GetID() const { return Id; }
	FORCEINLINE void SetAbilityInfo(FAbilityDataTable* InAbilityData) { AbilityData = InAbilityData; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 소유자 종류", meta=(DisplayPriority = 1))
	EEffectOwnerType EffectOwnership;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 접근 범위", meta=(DisplayPriority = 1))
	EEffectAccessRange EffectAccessRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 적용 대상", meta=(DisplayPriority = 1))
	EEffectApplyTarget EffectApplyTarget;
	
	UPROPERTY(EditAnywhere)
	int32 Id;
	
	FAbilityDataTable* AbilityData;
};
