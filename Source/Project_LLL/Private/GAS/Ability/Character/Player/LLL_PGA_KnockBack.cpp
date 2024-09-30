// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_KnockBack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Constant/LLL_GameplayTags.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Task/LLL_AT_Trace.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "Util/LLL_MathHelper.h"

void ULLL_PGA_KnockBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(CurrentEventData.TargetData, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (TriggerRequiredTag1.IsValid() && TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag1))
	{
		KnockBackTarget(TriggerEventData);
	}
	else if (TriggerRequiredTag2.IsValid() && TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag2))
	{
		KnockBackTarget(TriggerEventData);
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void ULLL_PGA_KnockBack::KnockBackTarget(const FGameplayEventData* TriggerEventData)
{
	bool KnockBackSuccess = false;
	
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	for (auto Actor : CurrentEventData.TargetData.Data[0]->GetActors())
	{
		const FVector AvatarLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
		const FVector Direction = (Actor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
		
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

		float KnockBackPower = 0;
		if (TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag1))
		{
			KnockBackPower = PlayerAttributeSet->GetKnockBackPower1();
		}
	
		if (TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag2))
		{
			KnockBackPower = PlayerAttributeSet->GetKnockBackPower2();
		}
		KnockBackPower *= PlayerAttributeSet->GetKnockBackPowerRate();
		KnockBackPower += PlayerAttributeSet->GetKnockBackPowerPlus();
		KnockBackPower += PlayerAttributeSet->GetBaseAttackKnockBackPowerPlus();
		
		if (ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(Actor))
		{
			FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(Direction, KnockBackPower);
			KnockBackActor->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);

			KnockBackSuccess = true;
		}
	}

	if (KnockBackSuccess)
	{
		PlayerASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_MORE_ATTACK_KNOCK_BACK));
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
