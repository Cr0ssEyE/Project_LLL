// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "LLL_RichTextToolTipDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_RichTextToolTipDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

	// 작업 중단
public:
	ULLL_RichTextToolTipDecorator(const FObjectInitializer& ObjectInitializer);
	
	// virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

protected:
	UPROPERTY(EditAnywhere, Category = "RichTextDecorator")
	FTextBlockStyle TextBlockStyle;

	UPROPERTY(EditAnywhere, Category = "RichTextDecorator")
	FTextBlockStyle TooltipTextBlockStyle;
	
};
