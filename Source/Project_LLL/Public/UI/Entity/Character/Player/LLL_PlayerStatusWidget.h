// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Entity/Character/Base/LLL_CharacterStatusWidget.h"
#include "LLL_PlayerStatusWidget.generated.h"

class UAbilitySystemComponent;
class ULLL_PlayerCharacterAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerStatusWidget : public ULLL_CharacterStatusWidget
{
	GENERATED_BODY()

public:
	virtual void UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet) override;
	void UpdateWidgetView(const UAbilitySystemComponent* CharacterASC) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> HealthTextBlock;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UProgressBar> SkillGaugeBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> SkillTextBlock;
};
