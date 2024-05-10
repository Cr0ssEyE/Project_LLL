
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EResolutionTypes : uint8
{
	SD UMETA(DisplayName = "854x480"),
	WSVGA UMETA(DisplayName = "1024×576"),
	HD UMETA(DisplayName = "1280x720"),
	FWXGA UMETA(DisplayName = "1366×768"),
	HDP UMETA(DisplayName = "1600x900"),
	FHD UMETA(DisplayName = "1920x1080"),
	QHD UMETA(DisplayName = "2560x1440")
};

UENUM(BlueprintType)
enum class EGraphicQuality : uint8
{
	None,
	Low,
	Middle,
	High
};