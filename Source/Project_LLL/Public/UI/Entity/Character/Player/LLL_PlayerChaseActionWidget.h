// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_PlayerChaseActionWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerChaseActionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	FORCEINLINE void SetPercent(float value) { Percent = value; } 
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<UImage> CircleProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, Category = "MaterialVariable")
	float Percent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, Category = "MaterialVariable")
	TObjectPtr<UMaterialInstanceDynamic> CircleProgressBarInst;

	UFUNCTION(BlueprintCallable)
	void SetCircleProgressBarValue(const float value);
};
