// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "DataAsset/LLL_RewardObjectDataAsset.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Interface/LLL_ProductObjectInterface.h"
#include "LLL_RewardObject.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInteractionDelegate, class ALLL_RewardObject*)

class ULLL_SelectRewardWidget;
class ULLL_ProductObjectPriceWidget;
class UWidgetComponent;
struct FRewardDataTable;

UCLASS()
class PROJECT_LLL_API ALLL_RewardObject : public ALLL_InteractiveObject, public ILLL_ProductObjectInterface
{
	GENERATED_BODY()
	
public:
	ALLL_RewardObject();

	virtual void InteractiveEvent(AActor* InteractedActor = nullptr) override;
	virtual void BeginPlay() override;
	virtual void ApplyProductEvent() override;
	virtual void SetInformation(const FRewardDataTable* Data, const uint32 Index = 0);

	FOnInteractionDelegate InteractionDelegate;

public:
	FORCEINLINE uint32 GetRewardDataIndex() const { return RewardIndex; }
	FORCEINLINE uint32 GetRewardDataID() const { return RewardData->ID; }
	
protected:
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
	
	const FRewardDataTable* RewardData;

	uint8 RewardIndex;
};
