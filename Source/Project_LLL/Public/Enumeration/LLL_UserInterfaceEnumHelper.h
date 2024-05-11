
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
	QHD UMETA(DisplayName = "2560×1440"),
	EnumEnd UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EResolutionTypes, EResolutionTypes::EnumEnd);

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
	JP UMETA(DisplayName = "JP"),
	EnumEnd UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(ELanguageType, ELanguageType::EnumEnd);

// 값을 폰트 사이즈로 사용
UENUM(BlueprintType)
enum class ESubtitleSize : uint8
{
	Disable = 0 UMETA(DisplayName = "Disable"),
	Small = 12 UMETA(DisplayName = "Small"),
	Medium = 24 UMETA(DisplayName = "Medium"),
	Large = 36 UMETA(DisplayName = "Large"),
	EnumEnd UMETA(Hidden)
};
ENUM_RANGE_BY_VALUES(ESubtitleSize, ESubtitleSize::Disable, ESubtitleSize::Small, ESubtitleSize::Medium, ESubtitleSize::Large);

// 값을 %로 사용
UENUM(BlueprintType)
enum class ESubtitleSpeed : uint8
{
	None = 0 UMETA(Hidden),
	VerySlow = 50 UMETA(DisplayName = "VerySlow"),
	Slow = 75 UMETA(DisplayName = "Slow"),
	Normal = 100 UMETA(DisplayName = "Normal"),
	Fast = 150 UMETA(DisplayName = "Fast"),
	VeryFast = 200 UMETA(DisplayName = "VeryFast"),
	EnumEnd UMETA(Hidden)
};
ENUM_RANGE_BY_VALUES(ESubtitleSpeed, ESubtitleSpeed::VerySlow, ESubtitleSpeed::Slow, ESubtitleSpeed::Normal, ESubtitleSpeed::Fast, ESubtitleSpeed::VeryFast);
