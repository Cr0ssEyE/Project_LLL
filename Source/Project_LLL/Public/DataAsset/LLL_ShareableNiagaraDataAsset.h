// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
#include "LLL_ShareableNiagaraDataAsset.generated.h"
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ShareableNiagaraDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Niagara", DisplayName = "몬스터 낙사시 투명 벽 파괴 이펙트")
	TObjectPtr<UNiagaraSystem> InvisibleWallCrashNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = "Niagara", DisplayName = "몬스터 표식 이펙트")
	TObjectPtr<UNiagaraSystem> MarkCountNiagaraSystem;
	
};
