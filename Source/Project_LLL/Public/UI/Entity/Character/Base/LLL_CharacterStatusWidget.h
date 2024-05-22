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
	virtual void UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthGaugeBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UProgressBar> ShieldGaugeBar;
};
