// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LLL_InteractionWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_InteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FORCEINLINE void PlayVisibleAnimation() { PlayAnimation(VisibleWidget); }
	FORCEINLINE void PlayHideAnimation() { PlayAnimation(HideWidget); }
	FORCEINLINE void SetInfoText(const FString& Text) const { InfoTextBlock->SetText(FText::FromString(Text)); }
	FORCEINLINE void SetInfoImageIcon(UTexture2D* Icon) const { InfoImageIcon->SetBrushFromTexture(Icon); }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> InfoTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> InfoImageIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> VisibleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HideWidget;
};
