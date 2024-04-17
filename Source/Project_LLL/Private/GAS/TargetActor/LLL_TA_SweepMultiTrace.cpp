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
	const UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC)
	{
		return FGameplayAbilityTargetDataHandle();
	}
	
	TArray<FHitResult> Results;
	const FVector SweepStartLocation =  SourceActor->GetActorLocation() + SourceActor->GetActorForwardVector() * TraceStartLocation;
	const FVector SweepEndLocation = SweepStartLocation + SourceActor->GetActorForwardVector() * TraceEndLocation;
	FQuat SweepQuat = SourceActor->GetActorQuat();

	if (TraceShape.ShapeType == ECollisionShape::Capsule)
	{
		SweepQuat *= FQuat(FVector::RightVector, FMath::DegreesToRadians(-90.0f));
	}
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ALLL_TA_SweepMultiTrace), false, SourceActor);
	Params.AddIgnoredActors(IgnoreActors.Array());
	
	GetWorld()->SweepMultiByChannel(
		Results,
		SweepStartLocation,
		SweepEndLocation,
		SweepQuat,
		TraceChannel,
		TraceShape,
		Params);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
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
			const FVector DistanceVector = SweepEndLocation - SweepStartLocation;
			const FVector SweepCenter = SweepStartLocation + DistanceVector / 2.0f;
			
			if(Results.IsEmpty())
			{
				FLLL_DebugDrawHelper::DrawDebugShapes(GetWorld(), BaseShape, SweepCenter, FColor::Blue, 2.f, BoxExtents, CapsuleExtents, SphereRadius, SweepQuat);
			}
			else
			{
				FLLL_DebugDrawHelper::DrawDebugShapes(GetWorld(), BaseShape, SweepCenter, FColor::Red, 2.f, BoxExtents, CapsuleExtents, SphereRadius, SweepQuat);
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
