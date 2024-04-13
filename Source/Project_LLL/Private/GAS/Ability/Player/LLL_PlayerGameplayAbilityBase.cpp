// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"

bool ULLL_PlayerGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	const ILLL_PlayerDependencyInterface* PlayerActor = Cast<ILLL_PlayerDependencyInterface>(CurrentActorInfo->AvatarActor);
	if(!PlayerActor)
	{
		return false;
	}
	
	return Result;
}
