// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_GlobalParameterDataAsset.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category = "Global", DisplayName = "낙사 이벤트 타임랙")
	float FallEventTimeDilation;

	UPROPERTY(EditDefaultsOnly, Category = "Global", DisplayName = "낙사 대상 넉백 속도")
	float DefaultFallKnockBackPower;
};
