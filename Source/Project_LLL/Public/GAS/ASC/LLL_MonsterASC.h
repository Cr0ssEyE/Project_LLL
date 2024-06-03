// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseASC.h"
#include "LLL_MonsterASC.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_MonsterASC : public ULLL_BaseASC
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULLL_MonsterASC();

	virtual FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
protected:
    // TODO: 플레이어랑 몬스터 ASC 분리하기
    virtual void OnFallableTagAdded(const FGameplayTag Tag, int32 count);
	virtual void OnMarkTagAdded(const FGameplayTag Tag, int32 count);
    virtual void CheckAbnormalEffect(const FGameplayEffectSpec& GameplayEffectSpec);

protected:
	FTimerHandle MarkTimerHandle;
	
};
