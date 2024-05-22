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

	UPROPERTY(EditDefaultsOnly, Category = "Sword", DisplayName = "메시")
	TObjectPtr<UStaticMesh> SwordMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Sword", DisplayName = "부착 소켓 이름")
	FName SwordAttachSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Sword", DisplayName = "위치")
	FVector SwordLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Sword", DisplayName = "회전")
	FRotator SwordRotation;

	UPROPERTY(EditDefaultsOnly, Category = "Sword", DisplayName = "크기")
	FVector SwordScale;
};
