// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
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

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void Dead() override;
	
public:
	virtual void Attack() override;
	virtual void Damaged() override;
	
	bool CanPlayAttackAnimation();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterBaseDataAsset;
	
	UPROPERTY(VisibleDefaultsOnly)
	float DetectDistance;

	UPROPERTY(VisibleDefaultsOnly)
	float FieldOfView;

public:
	UFUNCTION()
	void ToggleAIHandle(bool value);
};
