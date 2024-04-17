// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
#include "DataAsset/LLL_RewardObjectDataAsset.h"
#include "Interface/LLL_ProductObjectInterface.h"
#include "LLL_RewardObject.generated.h"

class ULLL_SelectRewardWidget;
class ULLL_ProductObjectPriceWidget;
class UWidgetComponent;

UCLASS()
class PROJECT_LLL_API ALLL_RewardObject : public ALLL_InteractiveObject, public ILLL_ProductObjectInterface
{
	GENERATED_BODY()
	
public:
	ALLL_RewardObject();

	virtual void InteractiveEvent() override;

	virtual void BeginPlay() override;

	virtual void ApplyProductEvent() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> RewardMesh;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsProduct;

	UPROPERTY(EditDefaultsOnly)
	float Price;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const ULLL_RewardObjectDataAsset> RewardObjectDataAsset;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> PriceWidgetComponent;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_ProductObjectPriceWidget> PriceWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_ProductObjectPriceWidget> PriceWidget;
};
