// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/Base/LLL_MGA_Attack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

void ULLL_MGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ALLL_MonsterBase* MonsterCharacter = Cast<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	if(!IsValid(MonsterCharacter))
	{
		return;
	}

	if(!IsValid(AttackMontage))
	{
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), AttackMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_MGA_Attack::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_MGA_Attack::OnInterruptedCallBack);

	PlayMontageTask->ReadyForActivation();
}
