// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_FeatherStorm.h"

#include "DataTable/LLL_AbilityDataTable.h"

void ULLL_PGA_FeatherStorm::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Log, TEXT("어빌리티 : %s, 랭크 : %s"), *GetName(), *StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(AbilityData->AbilityRank)));
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("%f, %f, %f"), static_cast<float>(AbilityData->AbilityRank), AbilityData->AbilityValue, AbilityData->ChangeValue));
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
