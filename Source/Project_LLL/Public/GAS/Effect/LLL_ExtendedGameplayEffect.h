// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Constant/LLL_FilePath.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
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
	
public:
	FORCEINLINE EEffectOwnerType GetOwnerShip() const { return EffectOwnership; }
	FORCEINLINE EEffectAccessRange GetAccessRange() const { return EffectAccessRange; }
	FORCEINLINE EEffectApplyTarget GetEffectApplyTarget() const { return EffectApplyTarget; }
	FORCEINLINE TArray<int32> GetID() const { return IdList; }
	FORCEINLINE const FAbilityDataTable* GetAbilityData() const { return AbilityData; }
	FORCEINLINE void SetAbilityInfo(const FAbilityDataTable* InAbilityData) { AbilityData = InAbilityData; }

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		FName AssetName = GetClass()->GetFName();
		AssetName = *AssetName.ToString().Replace(TEXT("_C"), TEXT(""));

		FName PrimaryAssetType;
		switch (EffectOwnership)
		{
		case EEffectOwnerType::Monster:
			PrimaryAssetType = PATH_MONSTER_EFFECTS;
			break;
		case EEffectOwnerType::Object:
			PrimaryAssetType = PATH_OBJECT_EFFECTS;
			break;
		case EEffectOwnerType::Player:
			PrimaryAssetType = PATH_PLAYER_EFFECTS;
			break;
		case EEffectOwnerType::Share:
			PrimaryAssetType = PATH_SHARE_EFFECTS;
			break;
		default:
			checkNoEntry();
		}
		
		return FPrimaryAssetId(PrimaryAssetType, AssetName);
	}
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 소유자 종류", meta=(DisplayPriority = 1))
	EEffectOwnerType EffectOwnership;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 접근 범위", meta=(DisplayPriority = 1))
	EEffectAccessRange EffectAccessRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "이펙트 적용 대상", meta=(DisplayPriority = 1))
	EEffectApplyTarget EffectApplyTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS", DisplayName = "고유 ID 리스트", meta=(DisplayPriority = 1))
	TArray<int32> IdList;
	
	const FAbilityDataTable* AbilityData;
};
