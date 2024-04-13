// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/MeleeMonster/SwordDash/LLL_MGA_Dash.h"

#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

void ULLL_MGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	Monster->LaunchCharacter(Monster->GetActorForwardVector() * MonsterAttributeSet->GetAttackDistance() * 10.0f, true, true);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
