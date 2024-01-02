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
	void EvadeAction(const FInputActionValue& Value);
	void AttackAction(const FInputActionValue& Value);
	void SkillAction(const FInputActionValue& Value);
	void InteractAction(const FInputActionValue& Value);
	void InventoryAction(const FInputActionValue& Value);
	void PauseAction(const FInputActionValue& Value);

	// 초기 데이터
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerBaseDataAsset> PlayerBaseDataAsset;
	/*
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> EvadeInputAction;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> AttackInputAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> SkillInputAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InteractionInputAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InventoryInputAction;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> PauseInputAction;
	*/
	
};
