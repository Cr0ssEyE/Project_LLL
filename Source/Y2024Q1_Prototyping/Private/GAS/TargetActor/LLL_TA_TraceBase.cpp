// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/LLL_TA_TraceBase.h"

#include "Abilities/GameplayAbility.h"


// Sets default values
ALLL_TA_TraceBase::ALLL_TA_TraceBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALLL_TA_TraceBase::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void ALLL_TA_TraceBase::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle ALLL_TA_TraceBase::MakeTargetData() const
{
	FGameplayAbilityTargetDataHandle DataHandle;
	return DataHandle;
}


