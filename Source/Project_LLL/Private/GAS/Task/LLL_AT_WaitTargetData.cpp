// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/LLL_AT_WaitTargetData.h"

#include "GAS/ASC/LLL_BaseASC.h"

ULLL_AT_WaitTargetData::ULLL_AT_WaitTargetData()
{
	bAllTargetHasMatching = false;
	bUsingOnce = true;
}

ULLL_AT_WaitTargetData* ULLL_AT_WaitTargetData::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AActor> RequiredClass, bool AllTargetHasMatching, bool UsingOnce)
{
	ULLL_AT_WaitTargetData* Task = NewAbilityTask<ULLL_AT_WaitTargetData>(OwningAbility);
	Task->RequiredActorClass = RequiredClass;
	Task->bAllTargetHasMatching = AllTargetHasMatching;
	Task->bUsingOnce = UsingOnce;
	return Task;
}

void ULLL_AT_WaitTargetData::Activate()
{
	Super::Activate();
	ULLL_BaseASC* LLLASC = Cast<ULLL_BaseASC>(AbilitySystemComponent);
	if (!IsValid(LLLASC))
	{
		EndTask();
	}

	LLLASC->TargetDataDelegate.AddDynamic(this, &ULLL_AT_WaitTargetData::OnTargetDataReceivedCallBack);
}

void ULLL_AT_WaitTargetData::OnDestroy(bool bInOwnerFinished)
{
	ULLL_BaseASC* LLLASC = Cast<ULLL_BaseASC>(AbilitySystemComponent);
	if (IsValid(LLLASC))
	{
		LLLASC->TargetDataDelegate.RemoveDynamic(this, &ULLL_AT_WaitTargetData::OnTargetDataReceivedCallBack);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void ULLL_AT_WaitTargetData::OnTargetDataReceivedCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	bool DataHasRequiredClass = false;
	UClass* ActorClass = RequiredActorClass;
	for(const auto Target : TargetDataHandle.Data[0]->GetActors())
	{
		if (Target->IsA(ActorClass))
		{
			DataHasRequiredClass = true;
		}
		else if (bAllTargetHasMatching)
		{
			DataHasRequiredClass = false;
			break;
		}
	}
	
	if (ShouldBroadcastAbilityTaskDelegates() && DataHasRequiredClass)
	{
		TargetDataReceivedDelegate.Broadcast(TargetDataHandle);
	}

	if(bUsingOnce)
	{
		EndTask();
	}
}
