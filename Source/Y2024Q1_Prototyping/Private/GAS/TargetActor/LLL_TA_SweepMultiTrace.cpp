// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/LLL_TA_SweepMultiTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "Util/LLL_DebugDrawHelper.h"

// Sets default values
ALLL_TA_SweepMultiTrace::ALLL_TA_SweepMultiTrace()
{
	
}

FGameplayAbilityTargetDataHandle ALLL_TA_SweepMultiTrace::TraceResult() const
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		return FGameplayAbilityTargetDataHandle();
	}
	
	switch (TraceTarget)
	{
	case ESelectTraceTarget::Player:
		break;
	case ESelectTraceTarget::Monster:
		break;
	case ESelectTraceTarget::AllEntity:
		break;
	default:
		checkNoEntry();
	}
	
	FName TraceProfile = FName(*UEnum::GetDisplayValueAsText(TraceTarget).ToString());
	
	TArray<FHitResult> Results;
	FVector SweepStartLocation =  SourceActor->GetActorLocation() +  SourceActor->GetActorForwardVector() * TraceStartLocation;
	FVector SweepEndLocation = SweepStartLocation +  SourceActor->GetActorForwardVector() * TraceEndLocation;
	FQuat SweepQuat = SourceActor->GetActorQuat();

	if (TraceShape.ShapeType == ECollisionShape::Capsule)
	{
		SweepQuat *= FQuat(FVector::RightVector, FMath::DegreesToRadians(-90.0f));
	}
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ALLL_TA_SweepMultiTrace), false, SourceActor);
	
	GetWorld()->SweepMultiByProfile(
		Results,
		SweepStartLocation,
		SweepEndLocation,
		SweepQuat,
		TraceProfile,
		TraceShape,
		Params);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		bool Debug = false;
		
		if (Cast<ALLL_PlayerBase>(SourceActor) && ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
		{
			Debug = true;
		}
		else if (Cast<ALLL_MonsterBase>(SourceActor) && ProtoGameInstance->CheckMonsterAttackDebug())
		{
			Debug = true;
		}

		if (Debug)
		{
			FVector DistanceVector = SweepEndLocation - SweepStartLocation;
			FVector SweepCenter = SweepStartLocation + DistanceVector / 2.0f;
			
			if(Results.IsEmpty())
			{
				FLLL_DebugDrawHelper::DrawDebugShapes(GetWorld(), BaseShape, SweepCenter, FColor::Blue, 2.f, BoxExtents, CapsuleExtents, SphereExtents, SweepQuat);
			}
			else
			{
				FLLL_DebugDrawHelper::DrawDebugShapes(GetWorld(), BaseShape, SweepCenter, FColor::Red, 2.f, BoxExtents, CapsuleExtents, SphereExtents, SweepQuat);
			}
		}
	}
#endif
	
	TArray<TWeakObjectPtr<AActor>> HitActors;
	if(!Results.IsEmpty())
	{
		for (const FHitResult& Result : Results)
		{
			AActor* HitActor = Result.GetActor();
			
			if (HitActor && !HitActors.Contains(HitActor))
			{
				HitActors.Add(HitActor);
			}
		}
	}
	
	FGameplayAbilityTargetData_ActorArray* ActorsData = new FGameplayAbilityTargetData_ActorArray();
	ActorsData->SetActors(HitActors);

	return FGameplayAbilityTargetDataHandle(ActorsData);
}
