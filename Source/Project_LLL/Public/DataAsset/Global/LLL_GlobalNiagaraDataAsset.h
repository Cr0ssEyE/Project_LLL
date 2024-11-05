// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "LLL_GlobalNiagaraDataAsset.generated.h"
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GlobalNiagaraDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Niagara|System", DisplayName = "몬스터 낙사시 투명 벽 파괴 이펙트")
	TObjectPtr<UNiagaraSystem> InvisibleWallCrashNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara|System", DisplayName = "몬스터 낙사시 트레일 이펙트")
	TObjectPtr<UNiagaraSystem> FallTrailNiagaraSystem;
	
	UPROPERTY(EditAnywhere, Category = "Niagara|System", DisplayName = "몬스터 낙사시 폭발 이펙트")
	TObjectPtr<UNiagaraSystem> FallExplosionNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara|System", DisplayName = "몬스터 낙사시 폭발 이펙트 지연 시간")
	float FallExplosionDelayTime;
	
	UPROPERTY(EditAnywhere, Category = "Niagara|System", DisplayName = "몬스터 표식 이펙트")
	TObjectPtr<UNiagaraSystem> StackCountNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara|System", DisplayName = "몬스터 출혈 이펙트 1")
	TObjectPtr<UNiagaraSystem> BleedingNiagaraSystem1;

	UPROPERTY(EditAnywhere, Category = "Niagara|System", DisplayName = "몬스터 출혈 이펙트 2")
	TObjectPtr<UNiagaraSystem> BleedingNiagaraSystem2;
};
