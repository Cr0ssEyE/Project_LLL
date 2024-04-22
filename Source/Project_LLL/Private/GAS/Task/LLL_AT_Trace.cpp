// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/LLL_AT_Trace.h"

#include "AbilitySystemComponent.h"
#include "GAS/TargetActor/LLL_TA_TraceBase.h"

ULLL_AT_Trace::ULLL_AT_Trace()
{
	bIsTraceSingleTick = true;
}

ULLL_AT_Trace* ULLL_AT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ALLL_TA_TraceBase> TargetActorClass, bool TraceSingleTick)
{
	ULLL_AT_Trace* NewTask = NewAbilityTask<ULLL_AT_Trace>(OwningAbility);
	NewTask->TargetActorClass = TargetActorClass;
	NewTask->bIsTraceSingleTick = TraceSingleTick;
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
		SpawnedTargetActor->SetIgnoreInfo(IgnoreActors);
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

	if (DataHandle.Data[0]->GetActors().Num() > 0)
	{
		TArray<TWeakObjectPtr<AActor>> TraceActors = DataHandle.Data[0]->GetActors();
		for(auto Actor : TraceActors)
		{
			IgnoreActors.Emplace(Actor.Get());
		}
	}
	
	if(bIsTraceSingleTick)
	{
		EndTask();
	}
}
