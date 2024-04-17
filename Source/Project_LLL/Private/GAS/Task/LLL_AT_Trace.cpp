// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/LLL_AT_Trace.h"

#include "AbilitySystemComponent.h"
#include "GAS/TargetActor/LLL_TA_TraceBase.h"

ULLL_AT_Trace::ULLL_AT_Trace()
{
	
}

ULLL_AT_Trace* ULLL_AT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ALLL_TA_TraceBase> TargetActorClass)
{
	ULLL_AT_Trace* NewTask = NewAbilityTask<ULLL_AT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	return NewTask;
}

void ULLL_AT_Trace::Activate()
{
	Super::Activate();
	SpawnAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void ULLL_AT_Trace::OnDestroy(bool bInOwnerFinished)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void ULLL_AT_Trace::SpawnAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<ALLL_TA_TraceBase>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>
		(
		TargetActorClass,
		FTransform::Identity,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn)
		);
	
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &ULLL_AT_Trace::OnTraceCompletedCallBack);
	}
}

void ULLL_AT_Trace::FinalizeTargetActor() const
{
	if (UAbilitySystemComponent* ASC = AbilitySystemComponent.Get(); IsValid(ASC))
	{
		const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawnTransform);

		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting();
	}
}

void ULLL_AT_Trace::OnTraceCompletedCallBack(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		TaskOnCompleteDelegate.Broadcast(DataHandle);
	}

	EndTask();
}
