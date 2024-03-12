// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/LLL_CharacterStatusWidget.h"
#include "LLL_PlayerStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerStatusWidget : public ULLL_CharacterStatusWidget
{
	GENERATED_BODY()

public:
	virtual void UpdateWidgetView(float MaxHealth, float CurrentHealth, float MaxShield, float CurrentShield) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> HealthTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> ShieldTextBlock;
};
