// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_PlayerWeaponDataAsset.generated.h"

USTRUCT()
struct FWeaponActionProperties
{
	GENERATED_BODY()
	FWeaponActionProperties()
	{
		ActionDamageValue = 0;
		ActionHitBoxSize = FVector::ZeroVector;
		ActionKnockBackValue = 0;
	}
	
	UPROPERTY(EditDefaultsOnly, DisplayName ="공격력 배율")
	float ActionDamageValue;

	UPROPERTY(EditDefaultsOnly, DisplayName ="공격 범위(가로, 세로, 높이)")
	FVector ActionHitBoxSize;

	UPROPERTY(EditDefaultsOnly, DisplayName ="넉백 수치")
	uint32 ActionKnockBackValue;
};
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

	// 
public:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", DisplayName = "무기 메시")
	TObjectPtr<UStaticMesh> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", DisplayName = "무기 부착 소켓 번호")
	uint32 WeaponHandSocketNum;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Attack", DisplayName = "무기 기본 공격력")
	float WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack", DisplayName = "무기 연속 액션 횟수")
	uint32 WeaponAttackActionCount;

	UPROPERTY(EditDefaultsOnly, Category = "Attack", DisplayName = "무기 액션 별 공격 설정 값")
	TArray<FWeaponActionProperties> WeaponActionProperties;
};
