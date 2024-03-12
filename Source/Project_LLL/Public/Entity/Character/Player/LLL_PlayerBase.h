// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_PlayerBase.generated.h"

class ALLL_PlayerWireHand;
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
class PROJECT_LLL_API ALLL_PlayerBase : public ALLL_BaseCharacter
{
	GENERATED_BODY()

	// 기본 상속 가상함수
public:
	ALLL_PlayerBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	// 외부 접근용
public:
	// TODO: GAS로 전환
	void AddInteractableObject(ALLL_InteractiveObject* Object);
	void RemoveInteractableObject(ALLL_InteractiveObject* RemoveObject);

	FORCEINLINE ULLL_PlayerUIManager* GetPlayerUIManager() const { return PlayerUIManager; }
	FORCEINLINE ALLL_PlayerWireHand* GetWireHand() const { return WireHandActor; }
	FVector GetMouseLocation() const;
	
	// 카메라
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;

	// 플레이어 관련 내부 객체
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerUIManager> PlayerUIManager;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ALLL_PlayerWireHand> WireHandActor;
	
	// 입력 액션 관련
private:
	void MoveAction(const FInputActionValue& Value);
	void DashAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void AttackAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void WireAction(const FInputActionValue& Value, EAbilityInputName InputName);
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
};
