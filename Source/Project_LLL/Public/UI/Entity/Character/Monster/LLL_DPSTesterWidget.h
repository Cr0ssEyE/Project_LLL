// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "LLL_DPSTesterWidget.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_DPSTesterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetWidgetText(const FString& Text) const { DPSTesterText->SetText(FText::FromString(Text)); }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<URichTextBlock> DPSTesterText;
};
