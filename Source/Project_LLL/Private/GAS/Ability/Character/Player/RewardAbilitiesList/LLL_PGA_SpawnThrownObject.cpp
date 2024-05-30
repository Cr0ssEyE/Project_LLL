// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_SpawnThrownObject.h"

#include "Components/CapsuleComponent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

void ULLL_PGA_SpawnThrownObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	ALLL_ThrownObject* ThrownObject = CastChecked<ALLL_ThrownObject>(Player->GetObjectPoolingComponent()->GetActor(ThrownObjectClass));
	
	const AActor* Target = bTargetIsInstigator ? CurrentEventData.Instigator : CurrentEventData.TargetData.Data[0]->GetActors()[0].Get();
	const FRotator Rotator = FRotationMatrix::MakeFromX(Player->GetActorLocation() - Target->GetActorLocation()).Rotator();
	const FVector OffsetLocation = Rotator.Vector() * Offset;
	
	ThrownObject->SetActorLocationAndRotation(Player->GetActorLocation() + OffsetLocation, Rotator);
	ThrownObject->AddActorLocalRotation(FRotator(0.0f, FMath::RandBool() ? 1.0f : -1.0f, 0.0f));
	ThrownObject->SetAbilityInfo(AbilityData, GetAbilityLevel());
	// Todo : 속도 정보 데이터화 필요
	ThrownObject->Throw(Player, Target, 2000);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
