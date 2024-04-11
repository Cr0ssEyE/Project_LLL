// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LLL_AbilityManageSubSystem.generated.h"

/**
 * 전반적인 어빌리티를 관리하는 서브시스템.
 * 코드만 적어놓고 별도로 생성이나 초기화 처리를 하지 않아도 인스턴스에서 불러올 수 있음. 왜지???
 */

class UGameplayEffect;
class UAbilitySystemComponent;
class ULLL_ObjectEffectStorageDataAsset;
class ULLL_MonsterEffectStorageDataAsset;
class ULLL_EffectStorageDataAsset;
class ALLL_BaseObject;
class ALLL_MonsterBase;
class ALLL_PlayerBase;
struct FGameplayTagContainer;

UCLASS()
class PROJECT_LLL_API ULLL_AbilityManageSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	ULLL_AbilityManageSubSystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
		
	// 유틸리티 모음?
public:
	void LoadEffectsFromPath(TArray<TSubclassOf<UGameplayEffect>>& Container, FName Path);
	TArray<TSubclassOf<UGameplayEffect>> FindEffectsByTag(TArray<TSubclassOf<UGameplayEffect>>& DataSet, const FGameplayTagContainer& EffectTag);
	
	// 어빌리티 OR 스탯 부여
public:
	TArray<TSubclassOf<UGameplayEffect>> FindPlayerEffectsByTag(ALLL_PlayerBase* TargetPlayer, const FGameplayTagContainer& EffectTag);
	TArray<TSubclassOf<UGameplayEffect>> FindMonsterEffectsByTag(ALLL_MonsterBase* TargetMonster, const FGameplayTagContainer& EffectTag);
	TArray<TSubclassOf<UGameplayEffect>> FindObjectEffectsByTag(ALLL_BaseObject* TargetObject, const FGameplayTagContainer& EffectTag);
	TArray<TSubclassOf<UGameplayEffect>> FindShareableEffectsByTag(const FGameplayTagContainer& EffectTag);
	
protected:
	// 플레이어에게 게임 진행 도중 일시적으로 어빌리티 OR 스탯을 부여하기 위해 사용하는 이펙트 모음. ex: 보상
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", DisplayName = "플레이어 이펙트 리스트")
	TArray<TSubclassOf<UGameplayEffect>> PlayerGameplayEffects;

	// 몬스터에게 게임 진행 도중 일시적으로 어빌리티 OR 스탯을 부여하기 위해 사용하는 이펙트 모음. ex: 강화 몬스터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", DisplayName = "플레이어 이펙트 리스트")
	TArray<TSubclassOf<UGameplayEffect>> MonsterGameplayEffects;

	// 오브젝트에게 게임 진행 도중 일시적으로 어빌리티 OR 스탯 등을 부여하기 위해 사용하는 이펙트 모음. ex: 부숴지는 오브젝트 중
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object", DisplayName = "플레이어 이펙트 리스트")
	TArray<TSubclassOf<UGameplayEffect>> ObjectGameplayEffects;

	// 두 분류 이상이 겹쳐서 사용 가능한 이펙트 모음 ex: 골드 드랍
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Share", DisplayName = "범용 이펙트 리스트")
	TArray<TSubclassOf<UGameplayEffect>> ShareableGameplayEffects;
};
