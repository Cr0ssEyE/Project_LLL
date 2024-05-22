// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC/LLL_BaseASC.h"

#include "Constant/LLL_GameplayTags.h"
#include "GAS/Ability/Character/Monster/LLL_MGA_SetFallableState.h"
#include "Interface/LLL_KnockBackInterface.h"

// Sets default values for this component's properties
ULLL_BaseASC::ULLL_BaseASC()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void ULLL_BaseASC::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<ILLL_KnockBackInterface>(GetAvatarActor()))
	{
		RegisterGameplayTagEvent(TAG_GAS_MONSTER_FALLABLE).AddUObject(this, &ULLL_BaseASC::OnFallableTagAdded);
	}
}

void ULLL_BaseASC::ReceiveTargetData(const UGameplayAbility* OwnerAbility, const FGameplayAbilityTargetDataHandle& TargetDataHandle) const
{
	FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(OwnerAbility->GetClass());
	
	if(Spec)
	{
		TargetDataDelegate.Broadcast(TargetDataHandle);
	}
}

void ULLL_BaseASC::OnFallableTagAdded(const FGameplayTag Tag, int32 count)
{
	if (Tag == TAG_GAS_MONSTER_FALLABLE && count > 0 && Cast<ILLL_KnockBackInterface>(GetAvatarActor()))
	{
		if (TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MONSTER_FALLABLE)))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("낙하 상태로 전환 %s"), *Tag.GetTagName().ToString()));
		}
	}
}



