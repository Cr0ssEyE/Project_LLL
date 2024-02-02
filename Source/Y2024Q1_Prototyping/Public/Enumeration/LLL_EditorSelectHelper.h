
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESelectShapeTypes
{
	Box,
	Capsule,
	Sphere
};

UENUM(BlueprintType)
enum class ESelectTraceTarget
{
	Player UMETA(DisplayName="Player"),
	Monster UMETA(DisplayName="Monster"),
	AllEntity UMETA(DisplayName="AllEntity")
};

DEFINE_ENUM_TO_STRING(ESelectTraceTarget)