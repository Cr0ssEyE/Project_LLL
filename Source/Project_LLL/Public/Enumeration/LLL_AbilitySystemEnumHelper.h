// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_AbilitySystemEnumHelper.generated.h"

// TMap<>과 같은 형식으로 키 값에 따라 고유한 어빌리티를 지정하는 경우 사용하기 위함
UENUM(BlueprintType)
enum class EAbilityInputName : uint8
{
	Attack UMETA(Displayname="공격"),
	Dash UMETA(Displayname="회피"),
	Skill UMETA(Displayname="스킬")
};

// 이펙트 회전 방향을 임의로 지정하는 경우 사용
UENUM(BlueprintType)
enum class EEffectFacingSetting : uint8
{
	None,
	LookPlayer,
	PlayerForward,
	LookCamera // 카메라 미구현
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

// 플레이어가 가하는 피해 유형에 따른 이벤트 발동을 제어하기 위해 사용
UENUM(BlueprintType)
enum class EEffectDamageEventType : uint8
{
	None UMETA(Displayname="피해 X"),
	BaseAttack UMETA(Displayname="기본 공격"),
	Eruria UMETA(Displayname="이누리아"),
	AbnormalStatus UMETA(Displayname="상태이상"),
	Skill UMETA(Displayname="스킬") // 현재는 스킬 자체에 피해를 가하는 기능이 없지만 추가 개발 상황 고려
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
enum class EAnimalType : uint8
{
	None UMETA(Hidden),
	Crow,
	Deer,
	Wolf,
	Horse,
	WildBoar,
	Eagle
};

UENUM(BlueprintType)
enum class EAbilityRank : uint8
{
	Normal,
	Rare,
	Epic,
	Synergy
};

UENUM(BlueprintType)
enum class EAbilityCategory : uint8
{
	Null,
	Feather,
	Bleeding,
	Blow
};

UENUM(BlueprintType)
enum class EAbilityValueType : uint8
{
	None UMETA(Hidden),
	Fixed = 1,
	Percent = 100
};

USTRUCT(BlueprintType)
struct FAbilityIconWrapper
{
	GENERATED_BODY()

public:
	FAbilityIconWrapper() {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> AbilityIcon;
};