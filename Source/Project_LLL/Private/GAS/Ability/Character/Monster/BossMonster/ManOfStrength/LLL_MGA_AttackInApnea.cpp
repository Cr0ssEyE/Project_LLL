// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/BossMonster/ManOfStrength/LLL_MGA_AttackInApnea.h"

#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

void ULLL_MGA_AttackInApnea::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!IsValid(AttackInApneaMontage))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s 어빌리티에 몽타주가 없음"), *GetName())
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackMontage"), AttackInApneaMontage, 1.0f, TEXT("1"));
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_MGA_AttackInApnea::SecondAttack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_MGA_AttackInApnea::SecondAttack);
	PlayMontageTask->ReadyForActivation();

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->SetAttacking(true);

	// Todo : 추후 데이터화 예정
	const float DashSpeed = 1000.0f;
	const float DashDistance = 200.0f;
	const FVector DashLocation = Monster->GetActorLocation() + Monster->GetActorForwardVector() * DashDistance;
	UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("AttackInApnea"), DashLocation, DashDistance / DashSpeed / Monster->CustomTimeDilation, nullptr, nullptr);
	MoveToLocationTask->ReadyForActivation();
}

void ULLL_MGA_AttackInApnea::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	Monster->SetAttacking(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_MGA_AttackInApnea::SecondAttack()
{
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackMontage"), AttackInApneaMontage, 1.0f, TEXT("2"));
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_MGA_AttackInApnea::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_MGA_AttackInApnea::OnInterruptedCallBack);
	PlayMontageTask->ReadyForActivation();

	const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());

	// Todo : 추후 데이터화 예정
	const float DashSpeed = 1000.0f;
	const float DashDistance = 200.0f;
	const FVector DashLocation = Monster->GetActorLocation() + Monster->GetActorForwardVector() * DashDistance;
	UAbilityTask_MoveToLocation* MoveToLocationTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("AttackInApnea"), DashLocation, DashDistance / DashSpeed / Monster->CustomTimeDilation, nullptr, nullptr);
	MoveToLocationTask->ReadyForActivation();
}
