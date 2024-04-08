// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_PlayerGoldWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerGoldWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	virtual void NativeConstruct() override;
	void UpdateWidget(float InGold) const;

	FORCEINLINE void PlayVisibleAnimation() { PlayAnimation(VisibleGold); }
	FORCEINLINE void PlayHideAnimation() { PlayAnimation(InvisibleGold); }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> VisibleGold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> InvisibleGold;
};
