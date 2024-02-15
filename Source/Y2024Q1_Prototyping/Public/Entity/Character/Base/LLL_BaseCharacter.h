// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterAnimInstance.h"
#include "DataAsset/LLL_BaseCharacterDataAsset.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "LLL_BaseCharacter.generated.h"

class ULLL_CharacterAttributeSetBase;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDeadDelegate, ALLL_BaseCharacter*)
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALLL_BaseCharacter();

	// 외부 접근용 함수
public:
	FORCEINLINE TObjectPtr<const ULLL_BaseCharacterDataAsset> GetCharacterDataAsset() const { return CharacterDataAsset; }
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	FORCEINLINE float GetTurnSpeed() const { return TurnSpeed; }
	FORCEINLINE float GetAttackDistance() const { return AttackDistance; }

protected:
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void SetDefaultInformation();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	// 캐릭터 상태 설정
public:
	virtual void Attack() {}
	virtual void Damaged() {}
	virtual void Dead();

	// 상태 체크용 변수
public:
	FORCEINLINE bool CheckCharacterIsDead() const { return bIsDead; }

	// 상태 체크용 델리게이트
public:
	FCharacterDeadDelegate CharacterDeadDelegate;

	// GAS 변수
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	// 캐릭터 공용 변수
protected:
	UPROPERTY(VisibleAnywhere)
	float AttackDistance;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsDead : 1;

	// 이동 관련 변수
protected:
	UPROPERTY(VisibleAnywhere)
	float AccelerateSpeed;

	UPROPERTY(VisibleAnywhere)
	float GroundFriction;

	UPROPERTY(VisibleAnywhere)
	float TurnSpeed;

	UPROPERTY(VisibleAnywhere)
	FVector MoveDirection;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BaseCharacterDataAsset> CharacterDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_BaseCharacterAnimInstance> CharacterAnimInstance;

protected:
	virtual void DeadMontageEndEvent();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	// 디버그용 함수
public:
	FORCEINLINE void SetCharacterDead() { bIsDead = true; }
	
	// 디버그용 변수
public:
	uint8 bIsSpawned : 1;
#endif
};
