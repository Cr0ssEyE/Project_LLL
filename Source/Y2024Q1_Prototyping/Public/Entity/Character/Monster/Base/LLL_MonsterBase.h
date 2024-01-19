// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_MonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_MonsterBase : public ALLL_BaseCharacter
{
	GENERATED_BODY()

public:
	ALLL_MonsterBase();
	
	FORCEINLINE float GetDetectDistance() const { return DetectDistance; }
	FORCEINLINE float GetFieldOfView() const { return FieldOfView; }

protected:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Dead() override;
	
public:
	virtual void Attack() override;
	virtual bool AttackAnimationIsPlaying() { return false; }

	void Damaged();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	float DetectDistance;

	UPROPERTY(VisibleDefaultsOnly)
	float FieldOfView;
};
