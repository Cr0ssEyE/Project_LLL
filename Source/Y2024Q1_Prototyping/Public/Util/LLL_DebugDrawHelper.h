#pragma once

#include "CoreMinimal.h"

class Y2024Q1_PROTOTYPING_API FLLL_DebugDrawHelper
{
public:
	static void DrawDebugShapes(const UWorld* InWorld, const ESelectShapeTypes ShapeTypes, FVector const& Center, FColor const& Color, float LifeTime=-1.f, FVector const& BoxExtent = FVector::ZeroVector, FVector2D const& CapsuleExtent = FVector2D::ZeroVector, FVector2D const& SphereExtent = FVector2D::ZeroVector, const FQuat& Rotation = FQuat::Identity, bool bPersistentLines = false, uint8 DepthPriority = 0, float Thickness = 0.f)
	{
		switch (ShapeTypes)
		{
		case ESelectShapeTypes::Box:
			DrawDebugBox(InWorld, Center, BoxExtent, Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
			break;
		case ESelectShapeTypes::Capsule:
			DrawDebugCapsule(InWorld, Center, CapsuleExtent.X, CapsuleExtent.Y, Rotation, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
			break;
		case ESelectShapeTypes::Sphere:
			DrawDebugSphere(InWorld, Center, SphereExtent.X, SphereExtent.Y, Color, bPersistentLines, LifeTime, DepthPriority, Thickness);
			break;
		default:
			checkNoEntry();
		}
	}
};
