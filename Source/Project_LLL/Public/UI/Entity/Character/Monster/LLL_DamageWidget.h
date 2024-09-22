// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "LLL_DamageWidget.generated.h"

#define START TEXT("<DamageCritical>")
#define END TEXT("</>")

UCLASS()
class PROJECT_LLL_API ULLL_DamageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE void PlayDamageAnimation() { PlayAnimation(AlphaChange); }
	FORCEINLINE void SetText(float Damage) { DamageText->SetText(FText::FromString(FString(START).Append(FString::SanitizeFloat(Damage)).Append(END))); }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<URichTextBlock> DamageText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> AlphaChange;
};
