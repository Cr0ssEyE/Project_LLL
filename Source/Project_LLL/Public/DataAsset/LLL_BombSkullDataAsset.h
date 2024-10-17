// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "LLL_BombSkullDataAsset.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_BombSkullDataAsset : public ULLL_MeleeMonsterDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "폭파 파티클")
	TObjectPtr<UNiagaraSystem> ExplodeParticle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "폭파 파티클 사이즈", meta=(DisplayPriority=1))
	FVector ExplodeParticleScale = FVector::OneVector;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "폭파 데미지 이펙트")
	TSubclassOf<UGameplayEffect> ExplodeDamageEffect;
};
