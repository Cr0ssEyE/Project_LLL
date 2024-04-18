// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_PlayerGoldWidget.generated.h"

class UTextBlock;


UCLASS()
class PROJECT_LLL_API ULLL_PlayerGoldWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	virtual void NativeConstruct() override;
	void UpdateInitWidget(float InGold) const;
	void UpdateGoldWidget(float TotalGold) const;

	FORCEINLINE void PlayShowInitGoldAnimation() { PlayAnimation(ShowInitGoldAnimation); }
	FORCEINLINE void PlayHideInitGoldAnimation() { PlayAnimation(HideInitGoldAnimation); }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidget))
	TObjectPtr<UTextBlock> InitGoldTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ShowInitGoldAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideInitGoldAnimation;
};
