﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Game/ProtoGameInstance.h"

class PROJECT_LLL_API FLLL_MathHelper
{
	// 범용
public:
	static FVector CalculateComponentFrontPoint(const USceneComponent* Component, const float Multiply)
	{
		const FVector CalculateResult = Component->GetComponentLocation() + Multiply * Component->GetForwardVector();
		return CalculateResult;
	}

	static FVector CalculateLaunchVelocity(const FVector& Direction, const float Multiply)
	{
		const FVector CalculateResult = Direction * Multiply * 10.f;
		return CalculateResult;
	}

	static FVector GetPredictedLocation(const AActor* Owner, const AActor* Target, float TargetSpeed, float PredictionRate)
	{
		const float Distance = Owner->GetDistanceTo(Target);
		const FVector PredictedMove = Target->GetVelocity() * (Distance / TargetSpeed);
		const FVector PredictedLocation = Target->GetActorLocation() + PredictedMove * PredictionRate;
		return PredictedLocation;
	}

	static bool IsInFieldOfView(const AActor* Owner, const AActor* Target, float Distance, float FieldOfView)
	{
		if (Distance < Owner->GetDistanceTo(Target))
		{
			return false;
		}
		
		FVector DirectionToTarget = Target->GetActorLocation() - Owner->GetActorLocation();
		DirectionToTarget.Normalize();

		const FVector OwnerForwardVector = Owner->GetActorForwardVector();
		const float DotProduct = FVector::DotProduct(OwnerForwardVector, DirectionToTarget);

		if (FMath::Acos(DotProduct) > FMath::DegreesToRadians(FieldOfView / 2.0f))
		{
			return false;
		}

		return true;
	}
	
	static bool CheckLaunchablePosition(const UWorld* World, const ACharacter* Owner, const float LaunchDistance, const FVector& LaunchDirection, const FName CollisionProfile)
	{
		FHitResult HitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);

		FVector LaunchablePosition = Owner->GetActorLocation() + LaunchDirection.GetSafeNormal() * LaunchDistance;
		World->LineTraceSingleByProfile(
			HitResult,
			Owner->GetActorLocation(),
			LaunchablePosition,
			CollisionProfile,
			Params
			);
		
		return !HitResult.GetActor();
	}
	
	// 플레이어
public:
	static float CalculatePlayerSkillGaugeIncrement(const float BaseValue, const float ComboAmplify, const float ItemAmplify)
	{
		const float CalculateResult = BaseValue * ComboAmplify * ItemAmplify;
		return CalculateResult;
	}

	static FVector CalculatePlayerLaunchableLocation(const UWorld* World, const ACharacter* Owner, const float LaunchDistance , const float CorrectionDistance, const FVector& LaunchDirection)
	{
		FHitResult CapsuleHitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);
		FVector LaunchLocation = Owner->GetActorLocation() + LaunchDirection.GetSafeNormal2D() * LaunchDistance;
		FVector2d CapsuleExtent = FVector2d(Owner->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.f, Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
		// 끼임 방지용 캐릭터 충돌 캡슐보다 1.2배 큰 반지름 체크
		World->SweepSingleByChannel(
			CapsuleHitResult,
			LaunchLocation,
			LaunchLocation,
			FQuat::Identity,
			ECC_WALL_ONLY,
			FCollisionShape::MakeCapsule(CapsuleExtent.X, CapsuleExtent.Y),
			Params
			);
		
		// 해당 범위에 아무런 액터도 충돌하지 않음 = 이동 거리만큼 이동해도 문제 없음
		if (!CapsuleHitResult.GetActor())
		{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(World->GetGameInstance()))
			{
				if(ProtoGameInstance->CheckPlayerDashDebug())
				{
					DrawDebugCapsule(World, LaunchLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Yellow, false, 2.f);
				}
			}
#endif
			return LaunchLocation;
		}

		FVector CorrectionLaunchLocation = LaunchLocation + LaunchDirection.GetSafeNormal2D() * CorrectionDistance;
		
		FHitResult CorrectionLocationHitResult;
		World->SweepSingleByChannel(
			CorrectionLocationHitResult,
			CorrectionLaunchLocation,
			CorrectionLaunchLocation,
			FQuat::Identity,
			ECC_WALL_ONLY,
			FCollisionShape::MakeCapsule(CapsuleExtent.X, CapsuleExtent.Y),
			Params
			);
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(World->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				DrawDebugCapsule(World, LaunchLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Red, false, 2.f);
				DrawDebugCapsule(World, CorrectionLaunchLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Cyan, false, 2.f);
			}
		}
#endif
		
		if (!CorrectionLocationHitResult.GetActor())
		{
			FHitResult CorrectionLineTraceHitResult;
			World->LineTraceSingleByChannel(
				CorrectionLineTraceHitResult,
				CorrectionLaunchLocation,
				Owner->GetActorLocation(),
				ECC_WALL_ONLY,
				Params
				);

			if (CorrectionLineTraceHitResult.GetActor())
			{
				CorrectionLaunchLocation = CorrectionLineTraceHitResult.ImpactPoint + LaunchDirection.GetSafeNormal2D() * CapsuleExtent.X;
				CorrectionLaunchLocation.Z = Owner->GetActorLocation().Z;
					
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
				if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(World->GetGameInstance()))
				{
					if(ProtoGameInstance->CheckPlayerDashDebug())
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 거리 보정 동작")));
						DrawDebugLine(World, Owner->GetActorLocation(), CorrectionLaunchLocation, FColor::Cyan, false, 2.f);
					}
				}
#endif
				return CorrectionLaunchLocation;
			}
		}

		Params.AddIgnoredActor(CapsuleHitResult.GetActor());
		FHitResult LineTraceHitResultFromLaunchLocation;
		World->LineTraceSingleByChannel(
			LineTraceHitResultFromLaunchLocation,
			LaunchLocation,
			Owner->GetActorLocation(),
			ECC_WALL_ONLY,
			Params
			);
		
		if (LineTraceHitResultFromLaunchLocation.GetActor())
		{
			float Distance = FVector::DistXY(LineTraceHitResultFromLaunchLocation.ImpactPoint, LaunchLocation);
			if (Distance >= CapsuleExtent.X)
			{
				Params.ClearIgnoredActors();
				Params.AddIgnoredActor(Owner);
				FVector SweepLocation = LineTraceHitResultFromLaunchLocation.ImpactPoint + LaunchDirection.GetSafeNormal2D() * CapsuleExtent.X;
				FHitResult ChinkCheckHitResult;
				
				World->SweepSingleByChannel(
				ChinkCheckHitResult,
			SweepLocation,
			SweepLocation,
			FQuat::Identity,
				ECC_WALL_ONLY,
				FCollisionShape::MakeCapsule(CapsuleExtent.X, CapsuleExtent.Y),
				Params
				);
				
				#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
                			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(World->GetGameInstance()))
                			{
                				if(ProtoGameInstance->CheckPlayerDashDebug())
                				{
                					DrawDebugCapsule(World, SweepLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Orange, false, 2.f);
                				}
                			}
                #endif

				if (!ChinkCheckHitResult.GetActor())
				{
					return SweepLocation;
				}
			} 
		}
		
		FHitResult LineTraceHitResultFromOwner;
		World->LineTraceSingleByChannel(
			LineTraceHitResultFromOwner,
			Owner->GetActorLocation(),
			LaunchLocation + LaunchDirection.GetSafeNormal2D() * CapsuleExtent.X * 2,
			ECC_WALL_ONLY,
			Params
			);
		
		// 앞서 캡슐로 긁어봐서 반드시 대상 검출 됨
		if (LineTraceHitResultFromOwner.GetActor())
		{
			// 대상과 충돌하는 위치 - 충돌체 반지름 만큼 위치 보정
			LaunchLocation = LineTraceHitResultFromOwner.ImpactPoint - LaunchDirection.GetSafeNormal2D() * CapsuleExtent.X;
			LaunchLocation.Z = Owner->GetActorLocation().Z;
				
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(World->GetGameInstance()))
			{
				if(ProtoGameInstance->CheckPlayerDashDebug())
				{
					DrawDebugLine(World, Owner->GetActorLocation(), LaunchLocation, FColor::Red, false, 2.f);
				}
			}
#endif
			return LaunchLocation;
		}

		// 여긴 사실상 들어오면 안됨
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 거리 계산 오류 발생")));
		DrawDebugLine(World, Owner->GetActorLocation(), LaunchLocation, FColor::Magenta, false, 2.f);
		DrawDebugCapsule(World, LaunchLocation, CapsuleExtent.Y, CapsuleExtent.X, FQuat::Identity, FColor::Magenta, false, 2.f);
		return LaunchLocation;
	}

	static float CalculatePlayerKnockBackCollisionCheckEndApproximation(const float KnockBackPower)
	{
		// 1000.f 기준 약 0.1초 뒤에 속도가 0에 근접함.
		// 5000.f 기준 약 0.2초 뒤에 속도가 0에 근접함.
		// 40000.f 기준 약 0.3초 뒤에 속도가 0에 근접함. 이 이상 플레이어의 공격으로 넉백 당하지 않을거라 생각해 0.3초를 최대 값으로 지정
		// 40000.f로 얼마나 멀리 날아가는지 궁금하시다면 BP_VelocityTestActor에서 Velocity 벡터 값중 아무 한군데 40000.f를 넣어보세요.
		// 위 CalculateLaunchVelocity 함수를 보면 아시겠지만 입력값에 *10 처리 하고 있어서 받는 값은 실제 속도의 1/10.
		
		const float Result = 0.2f;
		const float MinTime = 0.1f;
		const float MaxTime = 0.3f;

		if (KnockBackPower - 500.f <= 0)
		{
			const float Alpha = KnockBackPower / 500.f;
			const float TimeSubtractValue = FMath::Lerp(0.1f, 0.f, Alpha);
			return FMath::Clamp(Result - TimeSubtractValue, MinTime, Result);
		}

		if (KnockBackPower - 500.f > 0)
		{
			const float Alpha = 500.f / KnockBackPower;
			const float TimeAdditiveValue = FMath::Lerp(0.1f, 0.f, Alpha);
			return FMath::Clamp(Result + TimeAdditiveValue, Result, MaxTime);
		}
		
		return Result;
	}
private:
	
};
