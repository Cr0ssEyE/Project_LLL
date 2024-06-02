// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Ability/Character/Player/ComboSystem/LLL_PGA_ComboManagement.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "UI/Entity/Character/Player/LLL_PlayerComboWidget.h"

class ALLL_PlayerBase;

ULLL_PGA_ComboManagement::ULLL_PGA_ComboManagement()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboStackDuration = MaxComboStackDuration = CurrentComboCount = 0.f;
}

bool ULLL_PGA_ComboManagement::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return !IsActive();
}

void ULLL_PGA_ComboManagement::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug() || DebugGameInstance->CheckPlayerSkillDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("콤보 관리 어빌리티 발동")));
		}
	}
#endif
	
	PlayerAttributes = CastChecked<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	if(IsValid(PlayerAttributes))
	{
		CurrentComboCount = 0;
		MaxComboStackDuration = PlayerAttributes->GetMaxComboKeepingDuration();
		CurrentComboStackDuration = MaxComboStackDuration;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ComboManagement::ComboTimerTick);
}

void ULLL_PGA_ComboManagement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug() || DebugGameInstance->CheckPlayerSkillDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("콤보 관리 어빌리티 종료")));
		}
	}
#endif

	const FGameplayTagContainer SkillGaugeAmplifyByComboTag(TAG_GAS_COMBO_CHECK_AMPLIFY);
	GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(SkillGaugeAmplifyByComboTag);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ComboManagement::ComboTimerTick()
{
	CurrentComboStackDuration -= GetWorld()->GetDeltaSeconds();
	if(CurrentComboStackDuration > 0.f)
	{
		if(CurrentComboCount < PlayerAttributes->GetCurrentComboCount())
		{
			CurrentComboStackDuration = MaxComboStackDuration;
		}
		const FGameplayTagContainer SkillGaugeAmplifyByComboTag(TAG_GAS_COMBO_CHECK_AMPLIFY);
		GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(SkillGaugeAmplifyByComboTag);
	}
	else
	{
		if(CurrentComboCount == 0)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
			return;
		}
		
		CurrentComboStackDuration = MaxComboStackDuration;
		if(IsValid(ComboDivideEffect))
		{
			BP_ApplyGameplayEffectToOwner(ComboDivideEffect);
		}
	}
	
	CurrentComboCount = PlayerAttributes->GetCurrentComboCount();
	
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetCurrentActorInfo()->AvatarActor);
	ULLL_PlayerComboWidget* ComboWidget = Player->GetPlayerUIManager()->GetComboWidget();
	ComboWidget->SetComboText(CurrentComboCount);
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ComboManagement::ComboTimerTick);
	// TODO: UI 연결
}

