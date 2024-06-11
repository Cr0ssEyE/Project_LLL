// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
#include "Interface/LLL_DropGoldInterface.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "LLL_MonsterBase.generated.h"

class ULLL_MonsterAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MonsterBase : public ALLL_BaseCharacter, public ILLL_DropGoldInterface, public ILLL_KnockBackInterface
{
	GENERATED_BODY()

public:
	ALLL_MonsterBase();
	
	FORCEINLINE virtual void ResetKnockBackStack() override { StackedKnockBackVelocity = FVector::Zero(); StackedKnockBackedPower = 0.f; }
	FORCEINLINE void SetCharging(bool IsCharging) { bIsCharging = IsCharging; }
	
	FORCEINLINE virtual float GetKnockBackedPower() const override { return StackedKnockBackedPower; }
	FORCEINLINE bool IsCharging() const { return bIsCharging; }
	FORCEINLINE int32 GetId() const { return Id; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAttributeSet() override;
	
public:
	virtual void Damaged(AActor* Attacker = nullptr, bool IsDOT = false) override;
	virtual void Dead() override;
	virtual void AddKnockBackVelocity(FVector& KnockBackVelocity, float KnockBackPower) override;
	virtual void ApplyStackedKnockBack() override;

	void Attack() const;
	void Charge() const;
	void RecognizePlayerToAroundMonster() const;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterBaseDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_MonsterAttributeSet> MonsterAttributeSet;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> MonsterStatusWidgetComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> MaskMeshComponent;

	FVector StackedKnockBackVelocity;
	float StackedKnockBackedPower;
	int32 Id;
	uint8 bIsCharging : 1;
	
public:
	UFUNCTION()
	void ToggleAIHandle(bool value);

	// 이펙트 관련
public:
	void UpdateMarkVFX(uint8 NewCount = 0, uint8 MaxCount = 0);
	void UpdateBleedingVFX(bool ActiveState = true);
	void UpdateMonsterHitVFX();
	
	// 이펙트 관련
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> MarkVFXComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> BleedingVFXComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInstanceDynamic> HitEffectOverlayMaterialInstance;
	
//gold section
public:
	virtual void DropGold(const FGameplayTag tag, int32 data) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_DropGoldAttributeSet> DropGoldAttributeSet;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "Drop Gold어트리뷰트 초기화 이펙트")
	TSubclassOf<UGameplayEffect> DropGoldEffect;
};
