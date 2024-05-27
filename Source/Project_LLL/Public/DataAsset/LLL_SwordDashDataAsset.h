// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "LLL_SwordDashDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SwordDashDataAsset : public ULLL_MeleeMonsterDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "대시 데미지 이펙트")
	TSubclassOf<UGameplayEffect> DashDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "소드 메시")
	TObjectPtr<UStaticMesh> SwordMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "소드 부착 소켓 이름")
	FName SwordAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "소드 트랜스폼")
	FTransform SwordTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "어깨 갑옷 메시")
	TObjectPtr<UStaticMesh> ShoulderGuardMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "어깨 갑옷 부착 소켓 이름")
	FName ShoulderGuardAttachSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Accessory", DisplayName = "어깨 갑옷 트랜스폼")
	FTransform ShoulderGuardTransform;
};
