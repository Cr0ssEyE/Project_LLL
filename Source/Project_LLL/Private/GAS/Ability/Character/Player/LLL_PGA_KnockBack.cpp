// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_KnockBack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Task/LLL_AT_Trace.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "Util/LLL_MathHelper.h"

void ULLL_PGA_KnockBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ULLL_AT_WaitTargetData* TargetDataTask = ULLL_AT_WaitTargetData::CreateTask(this, ALLL_MonsterBase::StaticClass(), false, false);
	TargetDataTask->TargetDataReceivedDelegate.AddDynamic(this, &ULLL_PGA_KnockBack::OnTraceResultCallBack);
	TargetDataTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* TraceEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_ATTACK_HIT_CHECK_COMPLETE);
	TraceEndTask->EventReceived.AddDynamic(this, &ULLL_PGA_KnockBack::OnTraceEndCallBack);
	TraceEndTask->ReadyForActivation();
}

void ULLL_PGA_KnockBack::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		return;
	}
	
	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	const FVector AvatarLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
	
	for (auto Actor : TargetDataHandle.Data[0]->GetActors())
	{
		// 초기 구현은 MovementComponent의 LaunchCharacter 기반 물리 넉백으로 구현. 추후 방향성에 따른 수정 예정
		if (ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(Actor))
		{
			const FVector LaunchDirection = (Actor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
			const float ActionAmplify = KnockBackAmplifyChangeSection.Eval(CurrentEventData.EventMagnitude, KnockBackAmplifyChangeSection.RowName.ToString());
 			const float KnockBackPower = FLLL_MathHelper::CalculateKnockBackPower(PlayerCharacterAttributeSet, ActionAmplify);
			FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(LaunchDirection, KnockBackPower);
			
			KnockBackActor->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
		}
	}
	
	BP_ApplyGameplayEffectToTarget(TargetDataHandle, KnockBackEffect, CurrentEventData.EventMagnitude);
}

void ULLL_PGA_KnockBack::OnTraceEndCallBack(FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
