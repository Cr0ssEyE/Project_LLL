// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"

bool ULLL_PlayerGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	const ILLL_PlayerDependencyActorInterface* PlayerActor = Cast<ILLL_PlayerDependencyActorInterface>(CurrentActorInfo->AvatarActor);
	if(!PlayerActor)
	{
		return false;
	}
	
	return Result;
}
