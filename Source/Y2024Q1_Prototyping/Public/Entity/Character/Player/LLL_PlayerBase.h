// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "LLL_PlayerAnimInstance.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_PlayerBase.generated.h"

class ULLL_PlayerAttributeSet;
class ULLL_PlayerWeaponComponent;
class ULLL_PlayerAnimInstance;
class ALLL_InteractiveObject;
class ULLL_PlayerUIManager;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_PlayerBase : public ALLL_BaseCharacter
{
	GENERATED_BODY()

	// 기본 상속 가상함수
public:
	ALLL_PlayerBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 외부 접근용
public:
	void AddInteractableObject(ALLL_InteractiveObject* Object);
	void RemoveInteractableObject(ALLL_InteractiveObject* RemoveObject);

	// 카메라
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	// 플레이어 관련 내부 객체
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerAnimInstance> PlayerAnimInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerUIManager> PlayerUIManager;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerAttributeSet> PlayerAttributeSet;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerWeaponComponent> PlayerWeaponComponent;
	
	// 입력 액션 관련
private:
	void MoveAction(const FInputActionValue& Value);
	void DashAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void AttackAction(const FInputActionValue& Value);
	void SkillAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void InteractAction(const FInputActionValue& Value);
	void InteractiveTargetChangeAction(const FInputActionValue& Value);
	void InventoryAction(const FInputActionValue& Value);
	void PauseAction(const FInputActionValue& Value);

	void CharacterRotateToCursor();

	// 데이터 에셋
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerBaseDataAsset> PlayerDataAsset;
	
	// 공격 관련 함수
private:
	void SetAttackComboCheckState(bool Value);
	void SetAttackHitCheckState(bool Value);
	void AttackSequence();
	
	// 공격 관련 변수
private:
	uint32 CurrentComboActionCount;

	// 임시 변수
	uint32 MaxComboActionCount;
	
	uint8 bCheckAttackComboActionInput : 1;

	uint8 bIsAttackHitCheckOnGoing : 1;

	uint8 bIsAttackActionOnGoing : 1;

	// 상호작용 관련 변수
private:
	UPROPERTY()
	TArray<ALLL_InteractiveObject*> InteractiveObjects;

	UPROPERTY()
	int SelectedInteractiveObjectNum;

	UPROPERTY()
	uint32 InteractionRange;

	// 상태 관련 함수
protected:
	virtual void Dead() override;
	virtual void DeadMontageEndEvent() override;
	virtual void Attack() override;
	
	// 모션 캔슬시 사용 목적 함수
private:
	void ClearState();
	
};
