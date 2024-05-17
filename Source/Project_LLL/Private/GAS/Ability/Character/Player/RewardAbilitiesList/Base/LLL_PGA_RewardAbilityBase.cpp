// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"

#include "DataTable/LLL_AbilityDataTable.h"

void ULLL_PGA_RewardAbilityBase::SetAbilityInfo(const FAbilityDataTable* InAbilityData)
{
	AbilityData = InAbilityData;
	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("%f, %f, %f"), static_cast<float>(AbilityData->AbilityRank), AbilityData->AbilityValue, AbilityData->ChangeValue));
}

void ULLL_PGA_RewardAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("보상 어빌리티 발동 : %s, 등급 : %s"), *GetName(), *StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(AbilityData->AbilityRank)));
}
