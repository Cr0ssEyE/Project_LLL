// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterDataAsset.h"
#include "LLL_MonsterBaseDataAsset.generated.h"

enum class EMonsterWalkMaterialParameter : uint8;
class UBehaviorTree;
class UBlackboardData;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterBaseDataAsset : public ULLL_BaseCharacterDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "블랙보드")
	TObjectPtr<UBlackboardData> BlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "행동트리")
	TObjectPtr<UBehaviorTree> BehaviorTree;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "넉백 도중 충돌 발생시 애님 몽타주")
	TObjectPtr<UAnimMontage> KnockBackCollideMontage;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "스테이터스 게이지 위치")
	FVector StatusGaugeLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "스테이터스 게이지 크기")
	FVector2D StatusGaugeSize;

public:
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "발걸음 이벤트 파라미터 속성")
	TMap<TEnumAsByte<EPhysicalSurface>, EMonsterWalkMaterialParameter> StepEventParameterProperties;
};
