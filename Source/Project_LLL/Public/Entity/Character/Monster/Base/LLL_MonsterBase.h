// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
#include "Interface/LLL_DropGoldInterface.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "LLL_MonsterBase.generated.h"

struct FAbilityDataTable;
class ALLL_PlayerBase;
class UProjectileMovementComponent;
class ULLL_MonsterAttributeSet;
class ALLL_FloatingDamageActor;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MonsterBase : public ALLL_BaseCharacter, public ILLL_DropGoldInterface, public ILLL_KnockBackInterface
{
	GENERATED_BODY()

public:
	ALLL_MonsterBase();
	
	FORCEINLINE void SetCharging(const bool IsCharging) { bIsCharging = IsCharging; }
	FORCEINLINE void SetKnockBackSender(ALLL_MonsterBase* InKnockBackSender) { KnockBackSender = InKnockBackSender; }
	
	FORCEINLINE virtual float GetKnockBackedPower() const override { return bIsKnockBacking ? LastKnockBackPower : 0.0f; }
	FORCEINLINE bool IsCharging() const { return bIsCharging; }
	FORCEINLINE int32 GetId() const { return Id; }
	FORCEINLINE bool IsKnockBacking() const { return bIsKnockBacking; }
	FORCEINLINE float GetLastKnockBackPower() const { return LastKnockBackPower; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAttributeSet() override;
	virtual void SetFModParameter(EFModParameter FModParameter) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
public:
	virtual void Charge();
	virtual void Damaged(AActor* Attacker = nullptr, bool IsDOT = false, float Damage = 0) override;
	virtual void Dead() override;
	virtual void AddKnockBackVelocity(FVector& KnockBackVelocity, float KnockBackPower) override;
	
	virtual float GetChargeTimer() const;

	void Attack() const;
	void RecognizePlayerToAroundMonster() const;
	void ShowHitEffect();
	void ConnectOwnerDeadDelegate();
	void DisconnectOwnerDeadDelegate();
	void DamageKnockBackTarget(ALLL_PlayerBase* Player, const ALLL_MonsterBase* Monster);
	void DamageKnockBackCauser(ALLL_PlayerBase* Player);
	void Stun();
	void ShowDamageValue(const float Damage) const;

	// 이누리아 관련
public:
	FORCEINLINE void SetBleedingStack(const int32 InBleedingStack) { BleedingStack = InBleedingStack <= MaxBleedingStack ? InBleedingStack : MaxBleedingStack; }
	FORCEINLINE void SetMaxBleedingStack(const int32 InMaxBleedingStack) { MaxBleedingStack = InMaxBleedingStack; }
	FORCEINLINE void SetBleedingTransmissionOffencePower(const float InBleedingTransmissionOffencePower) { BleedingTransmissionOffencePower = InBleedingTransmissionOffencePower; }
	
	FORCEINLINE int32 GetBleedingStack() const { return BleedingStack; }
	FORCEINLINE bool GetBleedingTrigger() const { return bBleedingTrigger; }
	FORCEINLINE float GetMaxBleedingStack() const { return MaxBleedingStack; }

	void ToggleBleedingTrigger();
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterBaseDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_MonsterAttributeSet> MonsterAttributeSet;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> MonsterStatusWidgetComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<ALLL_FloatingDamageActor> FloatingDamageActor;

	int32 Id;
	uint8 bIsCharging : 1;
	FName AttributeInitId;
	uint8 bIsKnockBacking : 1;
	FVector LastKnockBackVelocity;
	float LastKnockBackPower;
	uint8 bStartKnockBackVelocity : 1;
	int32 DeflectCount;
	TObjectPtr<ALLL_MonsterBase> KnockBackSender;
	uint8 bKnockBackTargetDamaged : 1;
	uint8 bKnockBackCauserDamaged : 1;

	// 이누리아 관련
protected:
	int32 BleedingStack;
	int32 MaxBleedingStack;
	uint8 bBleedingTrigger : 1;
	float BleedingTransmissionOffencePower;
	uint8 bBleedingTransmissionTargetDamaged: 1;
	
public:
	UFUNCTION()
	void ToggleAIHandle(bool value);
	
	UFUNCTION()
	void OwnerCharacterDeadHandle(ALLL_BaseCharacter* Character);

	// 이펙트 관련
public:
	void UpdateStackVFX(uint8 NewCount = 0, uint8 MaxCount = 0);
	void UpdateBleedingVFX(bool ActiveState = true);
	void UpdateMonsterHitVFX();
	
	// 이펙트 관련
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> StackVFXComponent;

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
