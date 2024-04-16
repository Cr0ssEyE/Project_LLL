// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC/LLL_BaseASC.h"

// Sets default values for this component's properties
ULLL_BaseASC::ULLL_BaseASC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void ULLL_BaseASC::ReceiveTargetData(const UGameplayAbility* OwnerAbility, const FGameplayAbilityTargetDataHandle& TargetDataHandle) const
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(OwnerAbility->GetClass());
	if(Spec)
	{
		TargetDataDelegate.Broadcast(TargetDataHandle);
	}
}


