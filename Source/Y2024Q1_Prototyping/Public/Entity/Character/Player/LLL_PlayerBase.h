// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_PlayerBase.generated.h"

class ULLL_PlayerAnimInstance;
class ALLL_InteractiveObject;
class ULLL_PlayerUIManager;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class ULLL_PlayerBaseDataAsset;

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
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 외부 접근용
public:
	FORCEINLINE ULLL_PlayerBaseDataAsset* GetPlayerDataAsset() const { return PlayerBaseDataAsset; }
	
	void AddInteractableObject(ALLL_InteractiveObject* Object);
	void RemoveInteractableObject(ALLL_InteractiveObject* RemoveObject);

public:
	virtual void Attack() override;
	
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
	
	// 입력 액션 관련
private:
	void MoveAction(const FInputActionValue& Value);
	void DashAction(const FInputActionValue& Value);
	void AttackAction(const FInputActionValue& Value);
	void SkillAction(const FInputActionValue& Value);
	void InteractAction(const FInputActionValue& Value);
	void InteractiveTargetChangeAction(const FInputActionValue& Value);
	void InventoryAction(const FInputActionValue& Value);
	void PauseAction(const FInputActionValue& Value);

	void CharacterRotateToCursor();

	// 데이터 에셋
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerBaseDataAsset> PlayerBaseDataAsset;

	// 이동 관련 변수
private:
	UPROPERTY()
	float DashSpeed;
	
	// 돌진 관련 함수
private:
	void CheckDashElapsedTime();
	void CheckDashDelay();
	
	// 돌진 관련 변수
private:
	FTimerHandle DashStateCheckTimerHandle;

	UPROPERTY(VisibleDefaultsOnly)
	uint32 MaxDashCount;

	UPROPERTY()
	uint32 CurrentDashCount;
	
	float DashInputCheckTime;
	
	float DashCoolDownSeconds;
	
	float DashInvincibleTime;
	
	float DashDisabledTime;
	
	float DashElapsedTime;

	uint8 bIsInvincibleOnDashing : 1;

	// 공격 관련 함수
protected:
	
	// 상호작용 관련 변수
private:
	UPROPERTY()
	TArray<ALLL_InteractiveObject*> InteractiveObjects;

	UPROPERTY()
	int SelectedInteractiveObjectNum;

	UPROPERTY()
	uint32 InteractionRange;
	
};
