// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/ManOfStrength/LLL_MGA_AttackInApnea.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

void ULLL_MGA_AttackInApnea::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	int32 AttackCount = 4;
	
	if (!IsValid(AttackInApneaStartMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 어빌리티에 몽타주가 없음"), *GetName())
		return;
	}

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->PlayAnimMontage(AttackInApneaStartMontage);
	
	FTimerHandle AttackInApneaStartTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackInApneaStartTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, AttackCount]{
		if (AttackCount >= 2)
		{
			AttackMiddle(AttackCount - 1);
		}
		else
		{
			AttackEnd();
		}
	}), AttackInApneaStartMontage->GetPlayLength(), false);

	Monster->SetAttacking(true);
}

void ULLL_MGA_AttackInApnea::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->SetAttacking(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_MGA_AttackInApnea::AttackMiddle(int32 AttackCount)
{
	if (!IsValid(AttackInApneaMiddleMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 어빌리티에 몽타주가 없음"), *GetName())
		return;
	}
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->PlayAnimMontage(AttackInApneaMiddleMontage);
	
	FTimerHandle AttackInApneaMiddleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(AttackInApneaMiddleTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, AttackCount]{
		if (AttackCount >= 2)
		{
			AttackMiddle(AttackCount - 1);
		}
		else
		{
			AttackEnd();
		}
	}), AttackInApneaMiddleMontage->GetPlayLength(), false);
}

void ULLL_MGA_AttackInApnea::AttackEnd()
{
	if (!IsValid(AttackInApneaEndMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 어빌리티에 몽타주가 없음"), *GetName())
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackInApneaEndMontage"), AttackInApneaEndMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_MGA_AttackInApnea::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_MGA_AttackInApnea::OnInterruptedCallBack);
	PlayMontageTask->ReadyForActivation();
}
