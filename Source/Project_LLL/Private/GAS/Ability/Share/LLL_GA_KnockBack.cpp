// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Share/LLL_GA_KnockBack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Task/LLL_AT_Trace.h"
#include "Util/LLL_MathHelper.h"

ULLL_GA_KnockBack::ULLL_GA_KnockBack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_GA_KnockBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ULLL_AT_Trace* AttackTraceTask = ULLL_AT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->TaskOnCompleteDelegate.AddDynamic(this, &ULLL_GA_KnockBack::OnTraceResultCallBack);
	AttackTraceTask->ReadyForActivation();
}

void ULLL_GA_KnockBack::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	const FVector AvatarLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
	for (auto Actor : TargetDataHandle.Data[0]->GetActors())
	{
		// 초기 구현은 MovementComponent의 LaunchCharacter 기반 물리 넉백으로 구현. 추후 방향성에 따른 수정 예정
		ACharacter* MovableActor = Cast<ACharacter>(Actor);
		if (MovableActor)
		{
			const FVector LaunchDirection = (MovableActor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
			MovableActor->GetMovementComponent()->Velocity = FVector::Zero();
			MovableActor->LaunchCharacter(FLLL_MathHelper::CalculateLaunchVelocity(LaunchDirection, PlayerCharacterAttributeSet->GetKnockBackPower()), true, true);
		}

		// 만약 넉백 당하지는 않지만 넉백 관련 이벤트가 있는 대상일 경우를 위해 위와 별도 처리
		if (Cast<IAbilitySystemInterface>(MovableActor))
		{
			BP_ApplyGameplayEffectToTarget(TargetDataHandle, KnockBackEffect);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
