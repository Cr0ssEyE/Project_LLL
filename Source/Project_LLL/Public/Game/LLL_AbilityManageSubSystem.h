// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LLL_AbilityManageSubSystem.generated.h"

class ULLL_ExtendedGameplayEffect;
class UGameplayEffect;
class UAbilitySystemComponent;
class ULLL_ObjectEffectStorageDataAsset;
class ULLL_MonsterEffectStorageDataAsset;
class ULLL_EffectStorageDataAsset;
class ALLL_BaseObject;
class ALLL_MonsterBase;
class ALLL_PlayerBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncLoadEffectDelegate, TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>&, LoadedEffects);

/**
  어빌리티 관련 기능을 수행하는 서브시스템
 **/

UCLASS()
class PROJECT_LLL_API ULLL_AbilityManageSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	ULLL_AbilityManageSubSystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	FORCEINLINE TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> GetPlayerGameplayEffects() { return PlayerGameplayEffects; }
	FORCEINLINE TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> GetMonsterGameplayEffects() { return MonsterGameplayEffects; }
	FORCEINLINE TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> GetObjectGameplayEffects() { return ObjectGameplayEffects; }
	FORCEINLINE TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> GetShareableGameplayEffects() { return ShareableGameplayEffects; }

	// 어빌리티 OR 스탯 부여 이펙트 비동기 로딩
public:
	void ASyncLoadEffectsByTag(FAsyncLoadEffectDelegate Delegate, EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching = false);
	void ASyncLoadAttributeEffectsByTag(FAsyncLoadEffectDelegate Delegate, EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching = false);
	void ASyncLoadGrantAbilityEffectsByTag(FAsyncLoadEffectDelegate Delegate, EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching = false);
	
private:
	/** 플레이어에게 게임 진행 도중 일시적으로 어빌리티 OR 스탯을 부여하기 위해 사용하는 이펙트 모음. ex: 보상 \n
	/GAS/Effects/Character/Player 폴더 내부의 모든 ULLL_ExtendedGameplayEffect를 가져와 저장
	**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", DisplayName = "플레이어 이펙트 리스트", meta=(AllowPrivateAccess=true))
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> PlayerGameplayEffects;

	/** 몬스터에게 게임 진행 도중 일시적으로 어빌리티 OR 스탯을 부여하기 위해 사용하는 이펙트 모음. ex: 강화 몬스터 \n
	/GAS/Effects/Character/Monster 폴더 내부의 모든 ULLL_ExtendedGameplayEffect를 가져와 저장
	**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", DisplayName = "몬스터 이펙트 리스트", meta=(AllowPrivateAccess=true))
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> MonsterGameplayEffects;

	/** 오브젝트에게 게임 진행 도중 일시적으로 어빌리티 OR 스탯 등을 부여하기 위해 사용하는 이펙트 모음. ex: 부숴지는 오브젝트 \n
	/GAS/Effects/Character/Object 폴더 내부의 모든 ULLL_ExtendedGameplayEffect를 가져와 저장
	**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", DisplayName = "오브젝트 이펙트 리스트", meta=(AllowPrivateAccess=true))
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> ObjectGameplayEffects;

	/** 두 분류 이상이 겹쳐서 사용 가능한 이펙트 모음 ex: 골드 드랍 \n
	/GAS/Effects/Character/Share 폴더 내부의 모든 ULLL_ExtendedGameplayEffect를 가져와 저장
	**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Share", DisplayName = "범용 이펙트 리스트", meta=(AllowPrivateAccess=true))
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> ShareableGameplayEffects;

	// 내부적으로 작동하는 기능 모음?
private:
	void LoadEffectsFromPath(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& Container, FName PrimaryTypes);

	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& GetDataSetByOwner(EEffectOwnerType Owner);
};
