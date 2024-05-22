#pragma once

#include "CoreMinimal.h"

// TMap<>과 같은 형식으로 키 값에 따라 고유한 어빌리티를 지정하는 경우 사용하기 위함
UENUM(BlueprintType)
enum class EAbilityInputName : uint8
{
	Attack UMETA(Displayname="공격"),
	Skill UMETA(Displayname="스킬"),
	Chase UMETA(Displayname="추격"),
	Dash UMETA(Displayname="회피")
};

// 이펙트가 적용되는 대상을 지정하는 경우 사용
UENUM(BlueprintType)
enum class EEffectApplyTarget : uint8
{
	Self,
	Target
};

// 이펙트를 사용하는 대상을 분류하기 위해 사용
UENUM(BlueprintType)
enum class EEffectOwnerType : uint8
{
	Share UMETA(Displayname="공유"),
	Player UMETA(Displayname="플레이어"),
	Monster UMETA(Displayname="몬스터"),
	Object UMETA(Displayname="오브젝트")
};

// 이펙트가 접근하는 범위에 대한 설정. 필터링 목적
UENUM(BlueprintType)
enum class EEffectAccessRange : uint8
{
	None UMETA(Displayname="미지정(필터링 되지 않음)"),
	AttributeOnly UMETA(Displayname="스테이터스 변화"),
	AbilityOnly UMETA(Displayname="어빌리티 추가")
};

// 플레이어 어빌리티 종류 구분
UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None UMETA(Hidden),
	Crow,
	Deer,
	Wolf
};

UENUM(BlueprintType)
enum class EAbilityPart : uint8
{
	Common,
	Attack,
	Chase,
	Dash,
	ComboSkill
};

UENUM(BlueprintType)
enum class EAbilityRank : uint8
{
	Normal,
	Rare,
	Epic,
	Legend
};

UENUM(BlueprintType)
enum class EAbilityCategory : uint8
{
	Null,
	Feather,
	Critical,
	Marking,
	Bleeding
};