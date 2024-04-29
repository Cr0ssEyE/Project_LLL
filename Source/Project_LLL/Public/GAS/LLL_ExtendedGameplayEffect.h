// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "LLL_ExtendedGameplayEffect.generated.h"

class UAssetTagsGameplayEffectComponent;
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
	ULLL_ExtendedGameplayEffect();

	FORCEINLINE EEffectOwnerType GetOwnership() const { return EffectOwnership; }
	FORCEINLINE EEffectAccessRange GetAccessRange() const { return EffectAccessRange; }
	FORCEINLINE int32 GetID() const { return Id; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 소유자 종류", meta=(DisplayPriority = 1))
	EEffectOwnerType EffectOwnership;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 접근 범위", meta=(DisplayPriority = 1))
	EEffectAccessRange EffectAccessRange;

	UPROPERTY(EditAnywhere)
	int32 Id;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 유형")
	EAbilityType AbilityType;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 부위")
	EAbilityPart AbilityPart;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 등급")
	EAbilityRank AbilityRank;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 분류")
	EAbilityCategory AbilityCategory;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 이름")
	FName AbilityName;

	UPROPERTY(EditAnywhere, DisplayName = "능력 정보")
	FString AbilityInformation;

	UPROPERTY(EditAnywhere, DisplayName = "능력 수치")
	float AbilityValue;

	UPROPERTY(EditAnywhere, DisplayName = "수치 변화량")
	float ChangeValue;

	UPROPERTY(EditAnywhere, DisplayName = "획득 조건")
	EAbilityCategory RequireCategory;
};
