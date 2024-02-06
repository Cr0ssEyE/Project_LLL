
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESelectShapeTypes : uint8
{
	Box,
	Capsule,
	Sphere
};

UENUM(BlueprintType)
enum class ESelectTraceTarget : uint8
{
	Player UMETA(DisplayName="Player"),
	Monster UMETA(DisplayName="Monster"),
	AllEntity UMETA(DisplayName="AllEntity")
};