// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/MeleeMonster/SwordDash/LLL_MGA_Dash.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Interface/LLL_DashMonsterInterface.h"

void ULLL_MGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	if (ILLL_DashMonsterInterface* DashMonster = Cast<ILLL_DashMonsterInterface>(Monster))
	{
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(Monster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
		Monster->GetMovementComponent()->Velocity = FVector::Zero();
		Monster->LaunchCharacter(Monster->GetActorForwardVector() * MonsterAttributeSet->GetAttackDistance() * 10.0f, true, true);
		DashMonster->SetDash(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("대시할 수 있는 몬스터가 아닙니다"))
	}
	
	FTimerHandle HideTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(HideTimerHandle, this, &ULLL_MGA_Dash::EndDash, 0.2f, false);
}

void ULLL_MGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	if (ILLL_DashMonsterInterface* DashMonster = Cast<ILLL_DashMonsterInterface>(Monster))
	{
		Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
		DashMonster->SetDash(false);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_MGA_Dash::EndDash()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
