// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/TargetActor/LLL_TA_SweepMultiTrace.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/DamageEvents.h"
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
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ALLL_TA_SweepMultiTrace), false, SourceActor);
	
	GetWorld()->SweepMultiByProfile(
		Results,
		SweepStartLocation,
		SweepEndLocation,
		FQuat::Identity,
		TraceProfile,
		TraceShape,
		Params);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
		{
			if(Results.IsEmpty())
			{
				FLLL_DebugDrawHelper::DrawDebugShapes(GetWorld(), BaseShape, SweepStartLocation, FColor::Blue, 2.f, BoxExtents, CapsuleExtents, SphereExtents);
			}
			else
			{
				FLLL_DebugDrawHelper::DrawDebugShapes(GetWorld(), BaseShape, SweepStartLocation, FColor::Red, 2.f, BoxExtents, CapsuleExtents, SphereExtents);
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
			//TODO: 어트리뷰트 적용하면 삭제
			{
				FDamageEvent DamageEvent;
				HitActor->TakeDamage(100.f, DamageEvent, SourceActor->GetInstigatorController(), SourceActor);
			}
			
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
