
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

// UKismetInternationalizationLibrary::SetCurrentLanguageAndLocale 사용
UENUM(BlueprintType)
enum class ELanguageType : uint8
{
	KO UMETA(DisplayName = "KO"),
	EN UMETA(DisplayName = "EN"),
	JP UMETA(DisplayName = "JP")
};


// 값을 폰트 사이즈로 사용
UENUM(BlueprintType)
enum class ESubtitleSize : uint8
{
	Disable = 0,
	Small = 12,
	Medium = 24,
	Large = 36
};

// 값을 %로 사용
UENUM(BlueprintType)
enum class ESubtitleSpeed : uint8
{
	None = 0,
	VerySlow = 50,
	Slow = 75,
	Normal = 100,
	Fast = 200,
};
