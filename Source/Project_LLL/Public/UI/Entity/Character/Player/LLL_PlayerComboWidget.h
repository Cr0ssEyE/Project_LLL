// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_PlayerComboWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerComboWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void PlayComboTextAnimation() { PlayAnimation(ComboTextScaleChangeAnimation); }

	void SetComboText(float value);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> ComboCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ComboTextScaleChangeAnimation;
};
