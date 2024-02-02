// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/LLL_TA_SweepMultiTrace.h"

#include "AbilitySystemBlueprintLibrary.h"


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

	TArray<FHitResult> Results;
	FVector Origin = SourceActor->GetActorLocation();
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ALLL_TA_SweepMultiTrace), false, SourceActor);
	
	GetWorld()->SweepMultiByProfile(
		Results,
		Origin,
		Origin,
		FQuat::Identity,
		*EnumToString(TraceTarget),
		TraceShape,
		Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;
	if(!Results.Empty())
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
