// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownObjectDataAsset.h"
#include "LLL_ThrownFeatherDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ThrownFeatherDataAsset : public ULLL_ThrownObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Collision", DisplayName = "힛 콜리젼 사이즈")
	FVector HitCollisionSize;
	
	UPROPERTY(EditDefaultsOnly, Category = "Collision", DisplayName = "추적 깃털 투척 각도")
	float ChaseFeatherThrowAngleOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Collision", DisplayName = "추적 깃털 랜덤 배율")
	int32 ChaseFeatherThrowAngleRandomMultiply;
};
