// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_PlayerBase.generated.h"

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

public:
	ALLL_PlayerBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 카메라
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	// 입력 액션
private:
	void MoveAction(const FInputActionValue& Value);
	void DashAction(const FInputActionValue& Value);
	void AttackAction(const FInputActionValue& Value);
	void SkillAction(const FInputActionValue& Value);
	void InteractAction(const FInputActionValue& Value);
	void InventoryAction(const FInputActionValue& Value);
	void PauseAction(const FInputActionValue& Value);

	// 데이터 에셋
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerBaseDataAsset> PlayerBaseDataAsset;

	// 이동 관련 변수
private:
	UPROPERTY(VisibleDefaultsOnly)
	float MoveSpeed;

	UPROPERTY(VisibleDefaultsOnly)
	float AccelerateSpeed;

	UPROPERTY(VisibleDefaultsOnly)
	float DashSpeed;

	UPROPERTY(VisibleDefaultsOnly)
	float GroundFriction;

	// 돌진 관련 함수
private:
	void CheckDashInvincibilityTime();

	// 돌진 관련 변수
private:
	UPROPERTY(VisibleDefaultsOnly)
	uint32 DashCount;
	
	UPROPERTY(VisibleDefaultsOnly)
	float DashElapsedTime;
	
	UPROPERTY(VisibleDefaultsOnly)
	float DashInvincibleTime;

	UPROPERTY(VisibleDefaultsOnly)
	uint32 bIsInvincibleOnDashing : 1;
};
