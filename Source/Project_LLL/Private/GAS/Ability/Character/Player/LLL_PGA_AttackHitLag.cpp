// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_AttackHitLag.h"

#include "AbilitySystemComponent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"

ULLL_PGA_AttackHitLag::ULLL_PGA_AttackHitLag():
	bUseCurveDataTable(false),
	HitLagDuration(0.1),
	HitLagRate(0.2)
{
	
}

void ULLL_PGA_AttackHitLag::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	if ((TriggerRequiredTag.IsValid() && !TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag)) || !IsValid(HitLagTargetMontage))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
}

void ULLL_PGA_AttackHitLag::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}
	
	UAnimInstance* PlayerCharacterAnimInstance = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetMesh()->GetAnimInstance();

	float MontageHitLagRate = HitLagRate;
	if (bUseCurveDataTable)
	{
		MontageHitLagRate = HitLagRateByMotion.Eval(CurrentEventData.EventMagnitude, HitLagRateByMotion.RowName.ToString());
	}
	
	PlayerCharacterAnimInstance->Montage_SetPlayRate(HitLagTargetMontage, MontageHitLagRate);
	
	float MontageHitLagDuration = HitLagDuration;
	if (bUseCurveDataTable)
	{
		MontageHitLagDuration =  HitLagDurationByMotion.Eval(CurrentEventData.EventMagnitude, HitLagDurationByMotion.RowName.ToString());
	}
	
	FTimerHandle HitLagTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(HitLagTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]
	{
		if (!IsValid(GetWorld()))
		{
			return;
		}
		
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}), MontageHitLagDuration, false);
}

void ULLL_PGA_AttackHitLag::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
		return;
	}
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	float AttackSpeed = PlayerAttributeSet->GetAttackSpeed();
	AttackSpeed *= PlayerAttributeSet->GetFasterAttackAttackSpeedRate();
	
	PlayerCharacter->GetCharacterAnimInstance()->Montage_SetPlayRate(HitLagTargetMontage, AttackSpeed);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
