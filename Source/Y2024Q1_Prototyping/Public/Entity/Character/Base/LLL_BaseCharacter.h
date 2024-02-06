// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterAnimInstance.h"
#include "DataAsset/LLL_BaseCharacterDataAsset.h"
#include "GameFramework/Character.h"
#include "LLL_BaseCharacter.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterDeadDelegate, ALLL_BaseCharacter*)
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALLL_BaseCharacter();

	// 외부 접근용 함수
public:
	FORCEINLINE TObjectPtr<const ULLL_BaseCharacterDataAsset> GetCharacterDataAsset() const { return CharacterDataAsset; }
	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const { return ASC; }
	FORCEINLINE float GetTurnSpeed() const { return TurnSpeed; }
	FORCEINLINE float GetOffencePower() const { return OffensePower; }
	FORCEINLINE float GetAttackDistance() const { return AttackDistance; }

protected:
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void SetDefaultInformation();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 캐릭터 상태 설정
public:
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void Attack() {}
	virtual void Damaged() {}
	virtual void Dead();

	// 상태 체크용 변수
public:
	FORCEINLINE bool CheckCharacterIsDead() const { return bIsDead; }
	FORCEINLINE uint32 CheckCharacterHealth() const { return CurrentHealthAmount; }

	// 상태 체크용 델리게이트
public:
	FCharacterDeadDelegate CharacterDeadDelegate;

	// 캐릭터 공용 컴포넌트
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// 캐릭터 공용 변수
protected:
	UPROPERTY(VisibleAnywhere)
	uint32 MaxHealthAmount;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentHealthAmount;
	
	UPROPERTY(VisibleAnywhere)
	uint32 MaxShieldAmount;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentShieldAmount;
	
	UPROPERTY(VisibleAnywhere)
	uint32 OffensePower;
	
	UPROPERTY(VisibleAnywhere)
	float AttackDistance;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsDead : 1;

	// 이동 관련 변수
protected:
	UPROPERTY(VisibleAnywhere)
	float MoveSpeed;

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

	UPROPERTY(EditDefaultsOnly)
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
