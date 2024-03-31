// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
#include "Interface/LLL_DropGoldInterface.h"
#include "LLL_MonsterBase.generated.h"

class ULLL_MonsterAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MonsterBase : public ALLL_BaseCharacter, public ILLL_DropGoldInterface
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

//gold section
public:
	virtual void DropGold(const FGameplayTag tag, int32 data) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_DropGoldAttributeSet> DropGoldAttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "Drop Gold어트리뷰트 초기화 이펙트")
	TSubclassOf<UGameplayEffect> DropGoldEffect;
};
