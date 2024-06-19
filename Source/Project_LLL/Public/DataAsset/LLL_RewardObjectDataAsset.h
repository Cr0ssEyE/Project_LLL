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

	UPROPERTY(EditDefaultsOnly, Category = "Material", DisplayName = "메인 매쉬 머티리얼")
	TObjectPtr<UMaterialInstance> MainMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = "Material", DisplayName = "텍스처 매쉬 머티리얼")
	TObjectPtr<UMaterialInstance> TextureMaterialInst;

	UPROPERTY(EditDefaultsOnly, Category = "ProductData", DisplayName = "가격")
	float Price;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "플레이어 최대 체력 증가 이펙트")
	TSubclassOf<UGameplayEffect> MaxHPEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh", DisplayName = "보상 표시용 매시")
	TObjectPtr<UStaticMesh> RewardTextureMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Texture", DisplayName = "골드 보상 텍스쳐")
	TObjectPtr<UTexture> GoldTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "Texture", DisplayName = "능력 보상 텍스쳐")
	TObjectPtr<UTexture> AbilityTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "Texture", DisplayName = "최대 체력 보상 텍스쳐")
	TObjectPtr<UTexture> MaxHPTexture;
	
	UPROPERTY(EditDefaultsOnly, Category = "Texture", DisplayName = "강화 보상 텍스쳐")
	TObjectPtr<UTexture> EnhanceTexture;
};
