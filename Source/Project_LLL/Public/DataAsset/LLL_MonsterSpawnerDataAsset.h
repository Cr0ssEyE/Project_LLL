// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_MonsterSpawnerDataAsset.generated.h"

class UNiagaraSystem;
class UFMODEvent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterSpawnerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "스폰 사운드 이벤트")
	TObjectPtr<UFMODEvent> SpawnSoundEvent;

	UPROPERTY(EditDefaultsOnly, Category = "Timer", DisplayName = "스폰 타이머")
	float SpawnTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "스폰 파티클")
	TObjectPtr<UNiagaraSystem> SpawnParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "스폰 파티클 타이머")
	float SpawnParticleTimer;
};
