// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_PlayerChaseActionWidget.generated.h"

class UImage;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerChaseActionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	FORCEINLINE TObjectPtr<UImage> GetCircleProgressBar() { return CircleProgressBar; }
	
	UFUNCTION()
	void SetCircleProgressBarValue(const float value);

	void UpdateWidgetView(const UAbilitySystemComponent* CharacterASC);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<UImage> CircleProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, Category = "MaterialVariable")
	TObjectPtr<UMaterialInstanceDynamic> CircleProgressBarInstDynamic;

	UPROPERTY(VisibleAnywhere)
	float ChaseGauge;
};
