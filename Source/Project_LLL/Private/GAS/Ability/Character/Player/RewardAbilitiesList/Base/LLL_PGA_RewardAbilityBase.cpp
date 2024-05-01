// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"

#include "DataTable/LLL_AbilityDataTable.h"

void ULLL_PGA_RewardAbilityBase::SetAbilityInfo(const FAbilityDataTable* InAbilityData)
{
	AbilityRank = InAbilityData->AbilityRank;
	AbilityValue = InAbilityData->AbilityValue;
	ChangeValue = InAbilityData->ChangeValue;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("%f, %f, %f"), static_cast<float>(AbilityRank), AbilityValue, ChangeValue));
}

void ULLL_PGA_RewardAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("보상으로 얻은 어빌리티 발동"));
}
