
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EResolutionTypes : uint8
{
	SD UMETA(DisplayName = "854×480"),
	WSVGA UMETA(DisplayName = "1024×576"),
	HD UMETA(DisplayName = "1280×720"),
	FWXGA UMETA(DisplayName = "1366×768"),
	HDP UMETA(DisplayName = "1600×900"),
	FHD UMETA(DisplayName = "1920×1080"),
	QHD UMETA(DisplayName = "2560×1440")
};

UENUM(BlueprintType)
enum class EGraphicQuality : uint8
{
	None,
	Low,
	Middle,
	High
};

UENUM(BlueprintType)
enum class EDebugWidgetType : uint8
{
	Player,
	Monster,
	Object
};

UENUM(BlueprintType)
enum class ESettingWidgetType : uint8
{
	Display,
	Graphic,
	Sound,
	Gameplay
};