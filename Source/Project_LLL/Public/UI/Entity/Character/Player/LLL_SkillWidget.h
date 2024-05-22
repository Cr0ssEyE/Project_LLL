// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_SkillWidget.generated.h"

class UAbilitySystemComponent;
class ULLL_PlayerCharacterAttributeSet;
class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SkillWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void UpdateWidgetView(const UAbilitySystemComponent* CharacterASC) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> SkillGaugeText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UProgressBar> SkillGaugeBar;
};
