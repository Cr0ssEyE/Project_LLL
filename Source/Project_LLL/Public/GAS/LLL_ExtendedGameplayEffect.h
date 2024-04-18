// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "LLL_ExtendedGameplayEffect.generated.h"

class UAssetTagsGameplayEffectComponent;
enum class EEffectAccessRange : uint8;
enum class EEffectOwnerType : uint8;

/**
 * 원래는 그냥 UGameplayEffect를 사용하려고 했는데, 장기적으로 봤을 때 이펙트의 사용이나 적용 외적인 측면에서 설정이 어느정도 필요하다 판단하여 사용함.
 */
UCLASS()
class PROJECT_LLL_API ULLL_ExtendedGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	ULLL_ExtendedGameplayEffect();

	FORCEINLINE EEffectOwnerType GetOwnership() const { return EffectOwnership; }
	FORCEINLINE EEffectAccessRange GetAccessRange() const { return EffectAccessRange; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 소유자 종류", meta=(DisplayPriority = 1))
	EEffectOwnerType EffectOwnership;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 접근 범위", meta=(DisplayPriority = 1))
	EEffectAccessRange EffectAccessRange;
	
};
