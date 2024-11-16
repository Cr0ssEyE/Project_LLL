// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_Skill.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

void ULLL_PGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bSkillComplete = false;

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	
	UAnimMontage* SkillMontage = SkillMontages[Player->GetSkillEnuriaAnimalType()];
	if (!IsValid(SkillMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 어빌리티에 몽타주가 없음"), *GetName());
		return;
	}

	if (Player->IsSkillRotateToMouseCursor())
	{
		Player->RotateToMouseCursor();
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), SkillMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_Skill::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_Skill::OnInterruptedCallBack);
	PlayMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* SkillCompleteTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_SKILL_CHECK);
	SkillCompleteTask->EventReceived.AddDynamic(this, &ULLL_PGA_Skill::SkillCompleteCallBack);
	SkillCompleteTask->ReadyForActivation();
}

void ULLL_PGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (!bSkillComplete)
	{
		ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
		Player->ReadyToUseSkill();
	}

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	Player->SetSkillTriggered(false);
}

void ULLL_PGA_Skill::SkillCompleteCallBack(FGameplayEventData Payload)
{
	bSkillComplete = true;
}
