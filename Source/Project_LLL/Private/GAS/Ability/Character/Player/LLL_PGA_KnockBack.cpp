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
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	for (auto Actor : CurrentEventData.TargetData.Data[0]->GetActors())
	{
		const FVector AvatarLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
		const FVector Direction = (Actor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
		
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
		float KnockBackPower = PlayerAttributeSet->GetKnockBackPower();
		KnockBackPower *= PlayerAttributeSet->GetKnockBackPowerRate();
		KnockBackPower += PlayerAttributeSet->GetKnockBackPowerPlus();
		
		if (ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(Actor))
		{
			FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(Direction, KnockBackPower);
			KnockBackActor->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
		}
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
