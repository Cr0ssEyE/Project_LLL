// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "LLL_PlayerGoldComponent.h"
#include "DataAsset/LLL_CameraDataAsset.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Interface/LLL_PlayerDependencyInterface.h"
#include "LLL_PlayerBase.generated.h"

class ULLL_GameInstance;
class ULLL_AbnormalStatusAttributeSet;
class ULLL_PlayerSkillAttributeSet;
class ULLL_ObjectPoolingComponent;
class ALLL_PlayerChaseHand;
class ULLL_PlayerCharacterAttributeSet;
class ULLL_PlayerWeaponComponent;
class ULLL_PlayerAnimInstance;
class ALLL_InteractiveObject;
class ULLL_PlayerUIManager;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_PlayerBase : public ALLL_BaseCharacter, public ILLL_PlayerDependencyActorInterface
{
	GENERATED_BODY()

	// 기본 상속 가상함수
public:
	ALLL_PlayerBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void InitAttributeSet() override;
	virtual void SetFModParameter(EFModParameter FModParameter) override;
	
	virtual void Damaged(AActor* Attacker, bool IsDOT = false) override;
	virtual void Dead() override;
	
	// 외부 접근용
public:
	// TODO: GAS로 전환
	void AddInteractiveObject(ALLL_InteractiveObject* Object);
	void RemoveInteractiveObject(ALLL_InteractiveObject* RemoveObject);

	FORCEINLINE FVector GetMoveInputDirection() const { return MoveDirection; }
	FORCEINLINE bool GetMoveInputPressed() const { return bIsMoveInputPressed; }
	FORCEINLINE ULLL_PlayerUIManager* GetPlayerUIManager() const { return PlayerUIManager; }
	FORCEINLINE ALLL_PlayerChaseHand* GetChaseHand() const { return ChaseHandActor; }
	FORCEINLINE ULLL_PlayerGoldComponent* GetGoldComponent() const { return GoldComponent; }
	FORCEINLINE ULLL_ObjectPoolingComponent* GetObjectPoolingComponent() const { return ObjectPoolingComponent; }
	FORCEINLINE UWidgetComponent* GetChaseActionGaugeWidgetComponent() const { return ChaseActionGaugeWidgetComponent;}

	FORCEINLINE void SetCurrentCombo(int32 InCurrentCombo) { CurrentCombo = InCurrentCombo; }
	FORCEINLINE void SetMoveInputPressed(const FInputActionValue& Value, const bool Press) { bIsMoveInputPressed = Press; }
	
	FVector CheckMouseLocation();
	FVector GetLastCheckedMouseLocation() const { return LastCheckedMouseLocation; }
	void PlayerRotateToMouseCursor(float RotationMultiplyValue = 1.f, bool UseLastLocation = false);

protected:
	void TurnToMouseCursor();
	void MoveCameraToMouseCursor();
	void SetParameter(EFModParameter FModParameter, float value) const;
	
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

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerAnimInstance> PlayerAnimInstance;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ALLL_PlayerChaseHand> ChaseHandActor;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerSkillAttributeSet> SkillAttributeSet;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_AbnormalStatusAttributeSet> AbnormalStatusAttributeSet;
	
	// 입력 액션 관련
private:
	void MoveAction(const FInputActionValue& Value);
	void DashAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void AttackAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void ChaseAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void SkillAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void InteractAction(const FInputActionValue& Value);
	void InteractiveTargetChangeAction(const FInputActionValue& Value);
	void InventoryAction(const FInputActionValue& Value);
	void PauseAction(const FInputActionValue& Value);

	// 데이터 에셋
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const ULLL_PlayerBaseDataAsset> PlayerDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerCharacterAttributeSet> PlayerCharacterAttributeSet;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const ULLL_CameraDataAsset> CameraDataAsset;
	
	// 상호작용 관련 변수
private:
	UPROPERTY()
	TArray<TObjectPtr<ALLL_InteractiveObject>> InteractiveObjects;

	UPROPERTY()
	int SelectedInteractiveObjectNum;

	UPROPERTY()
	uint32 InteractionRange;

private:
	FVector LastCheckedMouseLocation;
	FRotator MouseDirectionRotator;
	float ToCursorRotationMultiplyValue;
	int32 LastAttackerMonsterId;
	int32 CurrentCombo;

	// 상태 관련 함수
protected:
	UFUNCTION()
	void DeadMotionEndedHandle();
	
	// 상태 관련 변수
protected:
	uint8 bIsMoveInputPressed : 1;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerGoldComponent> GoldComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_ObjectPoolingComponent> ObjectPoolingComponent;
	
	//UI 관련
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> ChaseActionGaugeWidgetComponent;

	// MPC 관련
protected:
	UFUNCTION()
	void DeactivatePPLowHP();

	UFUNCTION()
	void ActivatePPLowHP();

	UFUNCTION()
	void PlayLowHPAnimation();

	UPROPERTY(EditAnywhere)
	uint8 bIsLowHP : 1;

	UPROPERTY(EditDefaultsOnly)
	float ScalarValue;
};
