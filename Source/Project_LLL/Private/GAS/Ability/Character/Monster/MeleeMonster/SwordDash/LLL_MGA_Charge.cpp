// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/MeleeMonster/SwordDash/LLL_MGA_Charge.h"

#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/SwordDash/LLL_SwordDashAttributeSet.h"
#include "Interface/LLL_ChargeMonsterInterface.h"
#include "Util/LLL_ExecuteCueHelper.h"

void ULLL_MGA_Charge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());

	ILLL_ChargeMonsterInterface* ChargeMonster = CastChecked<ILLL_ChargeMonsterInterface>(Monster);
	ChargeMonster->SetCharge(true);

	const ULLL_SwordDashAttributeSet* SwordDashAttributeSet = CastChecked<ULLL_SwordDashAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_SwordDashAttributeSet::StaticClass()));

	FTimerHandle ChargeTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &ULLL_MGA_Charge::OnCompleteCallBack, SwordDashAttributeSet->GetChargeTimer());

	FLLL_ExecuteCueHelper::ExecuteCue(Monster, ChargeCueTag);
}

void ULLL_MGA_Charge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());

	ILLL_ChargeMonsterInterface* ChargeMonster = CastChecked<ILLL_ChargeMonsterInterface>(Monster);
	ChargeMonster->SetCharge(false);
}
