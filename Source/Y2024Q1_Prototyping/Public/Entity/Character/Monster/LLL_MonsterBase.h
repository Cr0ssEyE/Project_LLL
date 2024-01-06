// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_MonsterBase.generated.h"

class ULLL_MonsterDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_MonsterBase : public ALLL_BaseCharacter
{
	GENERATED_BODY()

public:
	ALLL_MonsterBase();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void Stun();
	
	FORCEINLINE bool AttackAnimationIsPlaying() const { return bAttackAnimationIsPlaying; }

public:
	void Attack();
	void AttackAnimationEndHandle();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterDataAsset> MonsterDataAsset;
	
	UPROPERTY(VisibleDefaultsOnly)
	uint8 bIsStun : 1;

	UPROPERTY(VisibleDefaultsOnly)
	uint8 bAttackAnimationIsPlaying : 1;
};
