// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_CharacterStatusWidget.generated.h"

class ULLL_CharacterAttributeSetBase;
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CharacterStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet);

protected:
	void UpdateFillMaterial();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "체력 바 베이스 머티리얼")
	TObjectPtr<UMaterial> HealthBarMaterial;

	// 메인 체력 바도 감소 애니메이션 만들고 싶다면 해당 값 사용
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "메인 체력 바 애니메이션 속도")
	// float HealthGaugeBarSpeed;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "감소한 값 애니메이션 속도")
	float HealthDecrementBarSpeed;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthGaugeBar;

protected:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> HealthBarDynamicMaterial;

	float CurrentHealthBarPercent;
	float CurrentSemiHealthBarPercent;

	FTimerHandle MaterialAnimTimerHandle;
};
