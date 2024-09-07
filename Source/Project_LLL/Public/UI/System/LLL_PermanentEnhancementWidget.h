// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/LLL_FocusWidgetInterface.h"
#include "LLL_PermanentEnhancementWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PermanentEnhancementWidget : public UUserWidget, public ILLL_FocusWidgetInterface
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
