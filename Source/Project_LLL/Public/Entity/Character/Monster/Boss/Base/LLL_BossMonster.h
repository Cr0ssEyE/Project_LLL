// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_BossMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "LLL_BossMonster.generated.h"

enum class EBossMonsterPattern : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_BossMonster : public ALLL_MonsterBase
{
	GENERATED_BODY()

public:
	ALLL_BossMonster();

	FORCEINLINE void SetChargeMontageKey(const EBossMonsterPattern InChargeMontageKey) { ChargeMontageKey = InChargeMontageKey; }
	FORCEINLINE void SetCurrentHavePatterns(const TArray<EBossMonsterPattern>& InCurrentHavePatterns) { CurrentHavePatterns = InCurrentHavePatterns; }
	
	FORCEINLINE EBossMonsterPattern GetChargeMontageKey() const { return ChargeMontageKey; }
	FORCEINLINE TArray<EBossMonsterPattern> GetCurrentHavePatterns() const { return CurrentHavePatterns; }

protected:
	virtual void BeginPlay() override;

	virtual void Dead() override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BossMonsterDataAsset> BossMonsterDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	EBossMonsterPattern ChargeMontageKey;

	UPROPERTY(VisibleAnywhere)
	TArray<EBossMonsterPattern> CurrentHavePatterns;
};
