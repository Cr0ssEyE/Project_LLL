// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "LLL_DPSTester.generated.h"

class ULLL_DPSTesterWidget;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_DPSTester : public ALLL_MonsterBase
{
	GENERATED_BODY()

public:
	ALLL_DPSTester();

protected:
	virtual void BeginPlay() override;
	virtual void Damaged(AActor* Attacker, bool IsDOT, float Damage) override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<ULLL_DPSTesterWidget> DPSTesterWidgetClass;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_DPSTesterWidget> DPSTesterWidget;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> DPSTesterWidgetComponent;

	UPROPERTY(EditAnywhere, DisplayName = "데미지 초기화 타이머")
	float ResetTimer;

	float TotalDamage;
	float StartTime;
	FTimerHandle TotalDamageTimerHandle;
};
