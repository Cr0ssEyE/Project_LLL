// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Base/LLL_CGA_StepCheck.h"

#include "AbilitySystemComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"

void ULLL_CGA_StepCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	UAbilitySystemComponent* ASC = CastChecked<ALLL_BaseCharacter>(ActorInfo->AvatarActor)->GetAbilitySystemComponent();
	FGameplayEffectContextHandle CueContextHandle = ASC->MakeEffectContext();
	CueContextHandle.AddSourceObject(this);
	FGameplayCueParameters CueParam;
	CueParam.EffectContext = CueContextHandle;
	
	ASC->ExecuteGameplayCue(TAG_GAS_STEP_SOUND, CueParam);
}
