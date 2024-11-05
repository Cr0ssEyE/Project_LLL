// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Task/LLL_AT_WaitOverlap.h"

ULLL_AT_WaitOverlap::ULLL_AT_WaitOverlap(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer),
	bUsingOnce(true)
{

}

void ULLL_AT_WaitOverlap::OnOverlapCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		FGameplayAbilityTargetData_SingleTargetHit * TargetData = new FGameplayAbilityTargetData_SingleTargetHit(SweepResult);

		FGameplayAbilityTargetDataHandle Handle;
		Handle.Data.Add(TSharedPtr<FGameplayAbilityTargetData>(TargetData));
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnOverlap.Broadcast(Handle);
		}

		if (bUsingOnce)
		{
			EndTask();
		}
	}
}

ULLL_AT_WaitOverlap* ULLL_AT_WaitOverlap::WaitForOverlap(UGameplayAbility* OwningAbility, bool UsingOnce)
{
	ULLL_AT_WaitOverlap* MyObj = NewAbilityTask<ULLL_AT_WaitOverlap>(OwningAbility);
	MyObj->bUsingOnce = UsingOnce;
	return MyObj;
}

void ULLL_AT_WaitOverlap::Activate()
{
	SetWaitingOnAvatar();

	UPrimitiveComponent* PrimComponent = GetComponent();
	if (PrimComponent)
	{
		PrimComponent->OnComponentBeginOverlap.AddDynamic(this, &ULLL_AT_WaitOverlap::OnOverlapCallBack);
	}
}

void ULLL_AT_WaitOverlap::OnDestroy(bool AbilityEnded)
{
	UPrimitiveComponent* PrimComponent = GetComponent();
	if (PrimComponent)
	{
		PrimComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ULLL_AT_WaitOverlap::OnOverlapCallBack);
	}

	Super::OnDestroy(AbilityEnded);
}

UPrimitiveComponent* ULLL_AT_WaitOverlap::GetComponent()
{
	UPrimitiveComponent * PrimComponent = nullptr;
	AActor* ActorOwner = GetAvatarActor();
	if (ActorOwner)
	{
		PrimComponent = Cast<UPrimitiveComponent>(ActorOwner->GetRootComponent());
		if (!PrimComponent)
		{
			PrimComponent = ActorOwner->FindComponentByClass<UPrimitiveComponent>();
		}
	}

	return PrimComponent;
}