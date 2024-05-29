// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_PersistentRevenge.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerThrownFeather.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

void ULLL_PGA_PersistentRevenge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	ALLL_PlayerThrownFeather* ThrownFeather = CastChecked<ALLL_PlayerThrownFeather>(Player->GetObjectPoolingComponent()->GetActor(ALLL_PlayerThrownFeather::StaticClass()));

	const AActor* Target = CurrentEventData.Instigator;
	const FRotator Rotator = FRotationMatrix::MakeFromX(Player->GetActorLocation() - Target->GetActorLocation()).Rotator();
	ThrownFeather->SetActorLocationAndRotation(Player->GetActorLocation(), Rotator);
	ThrownFeather->AddActorLocalRotation(FRotator(0.0f, FMath::RandBool() ? 1.0f : -1.0f, 0.0f));
	ThrownFeather->SetAbilityInfo(AbilityData, GetAbilityLevel());
	// Todo : 속도 정보 데이터화 필요
	ThrownFeather->Throw(Player, Target, 2000);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
