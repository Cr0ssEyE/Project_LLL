// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTable/LLL_FModParameterDataTable.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Ability/Monster/Base/LLL_MonsterGameplayAbilityBase.h"

bool ULLL_MonsterGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	// TODO: 여기서 제약 걸 수 있으면 한번 걸어주기. 플레이어 마냥 몬스터 관련 클래스에 공통되는 인터페이스 만들어서 붙여서 구분한다던가
	// const ALLL_MonsterBase* MonsterActor = Cast<ALLL_MonsterBase>(GetAvatarActorFromActorInfo());
	// if(!IsValid(MonsterActor))
	// {
	// 	return false;
	// }
	
	return Result;
	
}
