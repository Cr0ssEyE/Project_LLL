// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LLL_PlayerWeaponComponent.generated.h"


struct FWeaponActionProperties;
class ULLL_WeaponBaseDataAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class Y2024Q1_PROTOTYPING_API ULLL_PlayerWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULLL_PlayerWeaponComponent();
	
	FORCEINLINE void AddWeaponDamage(int Value) { Damage += Value; }

	void SetupWeaponInfo(const ULLL_WeaponBaseDataAsset* WeaponDataAsset);
	void StartMeleeWeaponHitCheck(uint32 CurrentActionCount);
	void StopMeleeWeaponHitCheck();
	
private:
	void MeleeWeaponHitCheck();
	
private:
	UPROPERTY()
	TArray<AActor*> DamagedActors;

	UPROPERTY()
	uint8 bIsHitCheckOnGoing : 1;
	
private:
	UPROPERTY(EditDefaultsOnly)
	uint32 Damage;

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxAttackActionCount;

	UPROPERTY(EditDefaultsOnly)
	uint32 CurrentAttackActionCount;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FWeaponActionProperties> AttackActionProperties;
	
};
