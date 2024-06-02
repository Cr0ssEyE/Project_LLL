// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_InteractiveObjectDataAsset.h"
#include "LLL_RewardObjectDataAsset.generated.h"

class ULLL_ProductObjectPriceWidget;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_RewardObjectDataAsset : public ULLL_InteractiveObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "가격 UI")
	TSubclassOf<ULLL_ProductObjectPriceWidget> PriceWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "위젯 위치")
	FVector PriceWidgetLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "위젯 크기")
	FVector2D PriceWidgetSize;

	UPROPERTY(EditDefaultsOnly, Category = "Material", DisplayName = "매쉬 머티리얼")
	TObjectPtr<UMaterialInstance> MaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = "ProductData", DisplayName = "가격")
	float Price;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "플레이어 최대 체력 증가 이펙트")
	TSubclassOf<UGameplayEffect> MaxHPEffect;
};
