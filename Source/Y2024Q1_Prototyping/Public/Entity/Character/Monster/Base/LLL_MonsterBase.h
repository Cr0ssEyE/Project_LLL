// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_MonsterBaseAnimInstance.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_MonsterBase.generated.h"

class ULLL_MonsterBaseAnimInstance;
class ULLL_MonsterBaseDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_MonsterBase : public ALLL_BaseCharacter
{
	GENERATED_BODY()

public:
	ALLL_MonsterBase();
	
	FORCEINLINE float GetDetectDistance() const { return MonsterBaseDataAsset->DetectDistance; }
	FORCEINLINE float GetFieldOfView() const { return MonsterBaseDataAsset->FieldOfView; }
	FORCEINLINE float GetAttackDistance() const { return MonsterBaseDataAsset->AttackDistance; }
	
	FORCEINLINE bool AttackAnimationIsPlaying();

protected:
	virtual void PostInitializeComponents() override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Dead() override;

public:
	void Stun();
	void Attack();

	void DamageToPlayer();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterBaseDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_MonsterBaseAnimInstance> MonsterBaseAnimInstance;
};
