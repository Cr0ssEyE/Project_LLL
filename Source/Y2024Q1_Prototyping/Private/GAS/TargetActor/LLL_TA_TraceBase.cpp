// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/LLL_TA_TraceBase.h"

#include "Abilities/GameplayAbility.h"


// Sets default values
ALLL_TA_TraceBase::ALLL_TA_TraceBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALLL_TA_TraceBase::SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes, FVector Extents)
{
	TraceTarget = NewTraceTarget;
	BaseShape = NewShapeTypes;
	if(!Extents.Length())
	{
		BoxExtents = Extents;
	}
}

void ALLL_TA_TraceBase::SetTraceInfo(const ESelectTraceTarget NewTraceTarget, const ESelectShapeTypes NewShapeTypes, FVector2D Radius)
{
	TraceTarget = NewTraceTarget;
	BaseShape = NewShapeTypes;
	if(Radius.X > 0)
	{
		SphereExtents = Radius;
		CapsuleExtents = Radius;
	}
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
	switch (BaseShape)
	{
	case ESelectShapeTypes::Box:
		TraceShape = FCollisionShape::MakeBox(BoxExtents);
		break;
	case ESelectShapeTypes::Capsule:
		TraceShape = FCollisionShape::MakeCapsule(CapsuleExtents.Y, CapsuleExtents.X);
		break;
	case ESelectShapeTypes::Sphere:
		TraceShape = FCollisionShape::MakeSphere(SphereExtents.X);
		break;
	default:
		checkNoEntry();
	}
	return TraceResult();
}

FGameplayAbilityTargetDataHandle ALLL_TA_TraceBase::TraceResult() const
{
	FGameplayAbilityTargetDataHandle DataHandle;
	return DataHandle;
}


