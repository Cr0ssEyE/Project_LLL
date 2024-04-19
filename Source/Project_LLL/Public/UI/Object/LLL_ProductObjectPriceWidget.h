// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "LLL_ProductObjectPriceWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECT_LLL_API ULLL_ProductObjectPriceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetPrice(float price) { PriceTextBlock->SetText(FText::FromString(FString::FromInt(price))); }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> PriceTextBlock;
};
