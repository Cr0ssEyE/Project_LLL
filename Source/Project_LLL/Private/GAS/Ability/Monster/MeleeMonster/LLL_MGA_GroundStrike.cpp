// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/MeleeMonster/LLL_MGA_GroundStrike.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Ability/Player/LLL_PGA_GroundStrike.h"

void ULLL_MGA_GroundStrike::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ALLL_MonsterBase* MonsterCharacter = Cast<ALLL_MonsterBase>(ActorInfo->AvatarActor.Get());
	if(!IsValid(MonsterCharacter))
	{
		return;
	}

	if(!IsValid(AbilityActionMontage))
	{
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), AbilityActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_MGA_GroundStrike::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_MGA_GroundStrike::OnInterruptedCallBack);

	PlayMontageTask->ReadyForActivation();
}
