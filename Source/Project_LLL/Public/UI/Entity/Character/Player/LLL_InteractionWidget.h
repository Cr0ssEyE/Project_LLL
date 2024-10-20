﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LLL_InteractionWidget.generated.h"

class UCanvasPanel;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_InteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FORCEINLINE void PlayVisibleAnimation() { PlayAnimation(VisibleWidget); }
	FORCEINLINE void PlayHideAnimation() { PlayAnimationReverse(VisibleWidget); }
	FORCEINLINE void SetInfoText(const FString& Text) const { InfoTextBlock->SetText(FText::FromString(Text)); }
	FORCEINLINE void SetInfoImageIcon(UTexture2D* Icon) const { InfoImageIcon->SetBrushFromTexture(Icon); }
	// FORCEINLINE void RenderNextInteractionPanel(const bool Value) const { ChangeInteractionPanel->SetRenderOpacity(Value); }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> InfoTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> InfoImageIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> VisibleWidget;
};
