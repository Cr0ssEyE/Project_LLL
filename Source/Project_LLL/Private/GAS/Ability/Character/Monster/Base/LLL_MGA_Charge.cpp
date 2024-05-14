// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/Base/LLL_MGA_Charge.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "Util/LLL_ExecuteCueHelper.h"

void ULLL_MGA_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(ChargeMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 어빌리티에 몽타주가 없음"), *GetName());
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ChargeMontage"), ChargeMontage, 1.0f, TEXT("Charge"));
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_MGA_Charge::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_MGA_Charge::OnInterruptedCallBack);

	PlayMontageTask->ReadyForActivation();

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	FLLL_ExecuteCueHelper::ExecuteCue(Monster, ChargeCueTag);
}
