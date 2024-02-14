// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_ComboManagement.h"

#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

ULLL_PGA_ComboManagement::ULLL_PGA_ComboManagement()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboStackDuration = MaxComboStackDuration = CurrentComboCount = 0.f;
}

void ULLL_PGA_ComboManagement::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("콤보 관리 어빌리티 발동")));
		}
	}
#endif
	
	PlayerAttributes = CastChecked<ULLL_PlayerAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));
	if(IsValid(PlayerAttributes))
	{
		CurrentComboCount = 0;
		MaxComboStackDuration = PlayerAttributes->GetMaxComboStackDuration();
		CurrentComboStackDuration = MaxComboStackDuration;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ComboManagement::ComboTimerTick);
}

void ULLL_PGA_ComboManagement::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("콤보 관리 어빌리티 종료")));
		}
	}
#endif
	
	if(!OnEndedEffects.IsEmpty())
	{
		for (auto EndedEffect : OnEndedEffects)
		{
			if(IsValid(EndedEffect.Key) && EndedEffect.Value == EEffectApplyTarget::Self)
			{
				BP_ApplyGameplayEffectToOwner(EndedEffect.Key);
			}
		}
	}
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
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ComboManagement::ComboTimerTick);
	// TODO: UI 연결
}

