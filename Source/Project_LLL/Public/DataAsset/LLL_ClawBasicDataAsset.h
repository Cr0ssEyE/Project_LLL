// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_MeleeMonsterDataAsset.h"
#include "LLL_ClawBasicDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ClawBasicDataAsset : public ULLL_MeleeMonsterDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "손톱 메시")
	TObjectPtr<UStaticMesh> ClawMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "손톱 부착 소켓 이름 (왼쪽)")
	FName LeftClawAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "손톱 부착 소켓 이름 (오른쪽)")
	FName RightClawAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "손톱 트랜스폼 (왼쪽)")
	FTransform LeftClawTransform;
	
	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "손톱 트랜스폼 (오른쪽)")
	FTransform RightClawTransform;
};
