// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_KnockBack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Task/LLL_AT_Trace.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "Util/LLL_MathHelper.h"

void ULLL_PGA_KnockBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerRequiredTag.IsValid() && !TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(CurrentEventData.TargetData, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	KnockBackTarget();
}

void ULLL_PGA_KnockBack::KnockBackTarget()
{
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	const FVector AvatarLocation = CurrentActorInfo->AvatarActor->GetActorLocation();

	for (auto Actor : CurrentEventData.TargetData.Data[0]->GetActors())
	{
		// 초기 구현은 MovementComponent의 LaunchCharacter 기반 물리 넉백으로 구현. 추후 방향성에 따른 수정 예정
		if (ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(Actor))
		{
			const FVector LaunchDirection = (Actor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
			const float ActionAmplify = KnockBackAmplifyChangeSection.Eval(CurrentEventData.EventMagnitude, KnockBackAmplifyChangeSection.RowName.ToString());
			const float KnockBackPower = FLLL_MathHelper::CalculateKnockBackPower(PlayerAttributeSet->GetKnockBackPower(), PlayerAttributeSet, ActionAmplify);
			FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(LaunchDirection, KnockBackPower);
			UE_LOG(LogTemp, Log, TEXT("넉백 수행(플레이어) : %f"), PlayerAttributeSet->GetKnockBackPower())
			KnockBackActor->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
		}
	}
	
	BP_ApplyGameplayEffectToTarget(CurrentEventData.TargetData, KnockBackEffect, CurrentEventData.EventMagnitude);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
