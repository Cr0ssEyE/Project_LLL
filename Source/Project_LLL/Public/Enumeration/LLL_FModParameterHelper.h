#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EFModParameter : uint8
{
	None UMETA(Hidden),
	PlayerWalkMaterialParameter,
	PlayerAttackCountParameter
};

UENUM(BlueprintType)
enum class EPlayerWalkMaterialParameter : uint8
{
	Dirt,
	Grass,
	Wood,
	Steel
};
