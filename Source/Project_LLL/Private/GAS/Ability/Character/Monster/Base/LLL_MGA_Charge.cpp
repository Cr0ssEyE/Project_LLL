// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/Base/LLL_MGA_Charge.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

void ULLL_MGA_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(ChargeMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 어빌리티에 몽타주가 없음"), *GetName());
		return;
	}

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	Monster->GetCharacterAnimInstance()->Montage_Play(ChargeMontage);

	FTimerHandle ChargeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}), MonsterAttributeSet->GetChargeTimer(), false);

	Monster->SetCharging(true);
}

void ULLL_MGA_Charge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->SetCharging(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
