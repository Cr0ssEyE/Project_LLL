// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_FeatherBarrage.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerThrownFeather.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

void ULLL_PGA_FeatherBarrage::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	ALLL_PlayerThrownFeather* ThrownFeather = CastChecked<ALLL_PlayerThrownFeather>(Player->GetObjectPoolingComponent()->GetActor(ALLL_PlayerThrownFeather::StaticClass()));

	ThrownFeather->SetActorLocationAndRotation(Player->GetActorLocation(), FRotationMatrix::MakeFromX(Player->GetActorLocation() - CurrentEventData.Instigator->GetActorLocation()).Rotator());
	ThrownFeather->AddActorLocalRotation(FRotator(0.0f, FMath::RandBool() ? 1.0f : -1.0f, 0.0f));
	ThrownFeather->Throw(Player, CurrentEventData.Instigator);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
