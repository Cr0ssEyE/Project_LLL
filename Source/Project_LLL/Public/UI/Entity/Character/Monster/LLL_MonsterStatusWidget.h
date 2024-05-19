// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Entity/Character/Base/LLL_CharacterStatusWidget.h"
#include "LLL_MonsterStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterStatusWidget : public ULLL_CharacterStatusWidget
{
	GENERATED_BODY()
public:
	virtual void UpdateWidgetView(const ULLL_CharacterAttributeSetBase* CharacterAttributeSet) const override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UProgressBar> ShieldGaugeBar;
};
