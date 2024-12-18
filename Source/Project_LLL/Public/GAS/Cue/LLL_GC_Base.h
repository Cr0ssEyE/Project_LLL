// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Burst.h"
#include "LLL_GC_Base.generated.h"

enum class EEffectFacingSetting : uint8;
struct FNiagaraInfo;
struct FFModInfo;
enum class EFModParameter : uint8;
class UFMODEvent;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_LLL_API ULLL_GC_Base : public UGameplayCueNotify_Burst
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	virtual void ReceiveSpawnResult(AActor* Target, const FGameplayCueNotify_SpawnResult& SpawnResult) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GCN Defaults", DisplayName = "파티클 페이싱 설정")
	EEffectFacingSetting EffectFacingTarget;
	
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "사운드 리스트")
	TArray<FFModInfo> FModInfos;
};
