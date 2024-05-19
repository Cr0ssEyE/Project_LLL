// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerChaseActionWidget.h"
#include "Components/Image.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHandAttributeSet.h"

void ULLL_PlayerChaseActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CircleProgressBarInstDynamic = CircleProgressBar->GetDynamicMaterial();
}

void ULLL_PlayerChaseActionWidget::SetCircleProgressBarValue(const float value) const
{
	CircleProgressBarInstDynamic->SetScalarParameterValue("Percent", value);
}

void ULLL_PlayerChaseActionWidget::UpdateWidgetView(const UAbilitySystemComponent* CharacterASC) const
{
	const ULLL_PlayerCharacterAttributeSet* PlayerCharactarAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(CharacterASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	/*const ULLL_PlayerChaseHandAttributeSet* PlayerChaseHandAttributeSet = Cast<ULLL_PlayerChaseHandAttributeSet>(CharacterASC->getAb);
	const float MaxCoolDownGauge = PlayerCharactarAttributeSet->GetChaseCoolDown();
	const float CurrentCoolDownGauge = PlayerChaseHandAttributeSet->GetGrabDuration();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("추격 액션 게이지: %f"), CurrentCoolDownGauge / MaxCoolDownGauge));*/
	
	//SetCircleProgressBarValue(CurrentCoolDownGauge / MaxCoolDownGauge);
}
