// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ComboSystem/LLL_PGA_ChangeComboAmplify_ByCombo.h"

#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

ULLL_PGA_ChangeComboAmplify_ByCombo::ULLL_PGA_ChangeComboAmplify_ByCombo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboAmplifyLevel = 1;
	CurrentAmplifyLevelComboCount = 0;
}

void ULLL_PGA_ChangeComboAmplify_ByCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ApplySkillGaugeAmplify();
}

void ULLL_PGA_ChangeComboAmplify_ByCombo::ApplySkillGaugeAmplify()
{
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	bool AmplifyChanged = false;
	const uint32 CurrentComboCount = PlayerAttributeSet->GetCurrentComboCount();
	
	const uint32 NextAmplifyNeededComboCount = ComboAmplifyChangeSection.Eval(CurrentComboAmplifyLevel + 1, ComboAmplifyChangeSection.RowName.ToString());
	if(CurrentComboCount >= NextAmplifyNeededComboCount && CurrentAmplifyLevelComboCount != NextAmplifyNeededComboCount)
	{
		AmplifyChanged = true;
		CurrentComboAmplifyLevel++;
		CurrentAmplifyLevelComboCount = NextAmplifyNeededComboCount;
	}

	const uint32 BeforeAmplifyNeededComboCount = ComboAmplifyChangeSection.Eval(FMath::Clamp(CurrentComboAmplifyLevel - 1, 1, CurrentComboAmplifyLevel), ComboAmplifyChangeSection.RowName.ToString());
	if(CurrentComboCount < BeforeAmplifyNeededComboCount && CurrentComboAmplifyLevel > 1)
	{
		AmplifyChanged = true;
		CurrentComboAmplifyLevel--;
		CurrentAmplifyLevelComboCount = BeforeAmplifyNeededComboCount;
	}
	
	if(AmplifyChanged)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("현재 콤보에 따른 스킬게이지 증가 배율 레벨 변경. 새로운 레벨: %d"), CurrentComboAmplifyLevel));
			}
		}
#endif
		
		BP_ApplyGameplayEffectToOwner(UpdateComboAmplifyEffect, CurrentComboAmplifyLevel);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
