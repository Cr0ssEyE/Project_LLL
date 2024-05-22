// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_PlayerChaseActionWidget.h"
#include "Components/Image.h"
#include "Constant/LLL_GameplayTags.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

void ULLL_PlayerChaseActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CircleProgressBarInstDynamic = CircleProgressBar->GetDynamicMaterial();
	ChaseGauge = 1.0f;
}

void ULLL_PlayerChaseActionWidget::SetCircleProgressBarValue(const float value)
{
	if (ChaseGauge == value)
	{
		return;
	}
	ChaseGauge = value;
	CircleProgressBarInstDynamic->SetScalarParameterValue("Percent", value);
}

void ULLL_PlayerChaseActionWidget::UpdateWidgetView(const UAbilitySystemComponent* CharacterASC)
{
	const ULLL_PlayerCharacterAttributeSet* PlayerCharactarAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(CharacterASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	const float MaxCoolDownGauge = PlayerCharactarAttributeSet->GetChaseCoolDown();
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	CharacterASC->FindAllAbilitiesWithTags(AbilitySpecHandles, FGameplayTagContainer(TAG_GAS_PLAYER_CHASER_THROW), true);

	if (AbilitySpecHandles.IsEmpty())
	{
		return;
	}
	const FGameplayAbilitySpec* Spec = CharacterASC->FindAbilitySpecFromHandle(AbilitySpecHandles[0]);

	if (AbilitySpecHandles.IsEmpty())
	{
		return;
	}

	const float CurrentCoolDownGauge = Spec->GetPrimaryInstance()->GetCooldownTimeRemaining();

	if (MaxCoolDownGauge <= 0)
	{
		return;
	}
	
	SetCircleProgressBarValue(1 - CurrentCoolDownGauge / MaxCoolDownGauge);
}
