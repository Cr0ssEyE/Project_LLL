// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_GlobalParameterDataAsset.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GlobalParameterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	ULLL_GlobalParameterDataAsset();
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Global|KnockBack", DisplayName = "낙사 이벤트 타임랙 배율")
	float FallEventTimeDilation;

	UPROPERTY(EditDefaultsOnly, Category = "Global|KnockBack", DisplayName = "낙사 이벤트 타임랙 지속시간")
	float FallEventDuration;
	
	UPROPERTY(EditDefaultsOnly, Category = "Global|KnockBack", DisplayName = "낙사 대상 넉백 속도")
	float DefaultFallKnockBackPower;

	UPROPERTY(EditDefaultsOnly, Category = "Global|KnockBack", DisplayName = "낙사 요구 속도값")
	float FallRequiredVelocityLength;

	UPROPERTY(EditDefaultsOnly, Category = "Global|KnockBack", DisplayName = "몬스터 무게당 감속량(*)")
	float DecreaseVelocityPerWeight;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Global|Reward", DisplayName = "황금사과 체력 증가 이펙트")
	TSubclassOf<UGameplayEffect> GoldAppleIncreaseHpEffect;
	
};
