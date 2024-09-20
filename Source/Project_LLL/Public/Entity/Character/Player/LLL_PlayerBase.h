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

class ALLL_PlayerController;
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDissolveCompleteDelegate, bool, IsDrop);

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
	
	virtual void Damaged(AActor* Attacker = nullptr, bool IsDOT = false) override;
	virtual void Dead() override;
	
	// 외부 접근용
public:
	// TODO: GAS로 전환
	void AddInteractiveObject(ALLL_InteractiveObject* Object);
	void RemoveInteractiveObject(ALLL_InteractiveObject* RemoveObject);

	void CharacterUnDissolveBegin();

public:
	FORCEINLINE FVector GetMoveInputDirection() const { return MoveDirection; }
	FORCEINLINE bool GetMoveInputPressed() const { return bIsMoveInputPressed; }
	FORCEINLINE UCameraComponent* GetPlayerCamera() const { return Camera; }
	FORCEINLINE USpringArmComponent* GetPlayerSpringArm() const { return SpringArm; }
	FORCEINLINE ULLL_PlayerUIManager* GetPlayerUIManager() const { return PlayerUIManager; }
	FORCEINLINE ULLL_PlayerGoldComponent* GetGoldComponent() const { return GoldComponent; }
	FORCEINLINE ULLL_ObjectPoolingComponent* GetObjectPoolingComponent() const { return ObjectPoolingComponent; }
	FORCEINLINE FVector GetLastCheckedMouseLocation() const { return LastCheckedMouseLocation; }
	FORCEINLINE bool IsSkillRotateToMouseCursor() const { return bSkillRotateToMouseCursor; }

	FORCEINLINE void SetCurrentCombo(const int32 InCurrentCombo) { CurrentCombo = InCurrentCombo; }
	FORCEINLINE void SetMoveInputPressed(const bool Press) { bIsMoveInputPressed = Press; }
	FORCEINLINE void SetSkillCoolTime(const float InSkillCoolTime) { SkillCoolTime = InSkillCoolTime; }
	FORCEINLINE void SetSkillRotateToMouseCursor(const bool SkillRotateToMouseCursor) { bSkillRotateToMouseCursor = SkillRotateToMouseCursor; }
	
	FVector CheckMouseLocation();
	void RotateToMouseCursor(float RotationMultiplyValue = 1.f, bool UseLastLocation = false);
	void StartCameraMoveToCursor(const ALLL_PlayerController* PlayerController = nullptr);
	void PauseCameraMoveToCursor() const;
	void ReadyToUseSkill();
	
	int32 GetEnuriaCount() const;

public:
	// 이누리아 관련 함수
	FORCEINLINE int32 GetChargedFeatherCount() const { return ChargedFeatherCount; }
	FORCEINLINE float GetFeatherSpawnStartTime() const { return FeatherSpawnStartTime; }
	FORCEINLINE int32 GetDeflectCount() const { return DeflectCount; }
	FORCEINLINE float GetKnockBackTransmissionOffencePower() const { return KnockBackTransmissionOffencePower; }
	FORCEINLINE float GetKnockBackTransmissionKnockBackPower() const { return KnockBackTransmissionKnockBackPower; }
	FORCEINLINE float GetQuadrupleHitKnockBackPower() const { return QuadrupleHitKnockBackPower; }
	FORCEINLINE float GetFasterKnockBackSpeedRate() const { return FasterKnockBackSpeedRate; }
	FORCEINLINE float GetIncreaseKnockBackDamageByEnuriaCountDamageRate() const { return IncreaseKnockBackDamageByEnuriaCountDamageRate; }
	FORCEINLINE float GetRangeKnockBackKnockBackPower() const { return RangeKnockBackKnockBackPower; }
	FORCEINLINE int32 GetBleedingTransmissionStack() const { return BleedingTransmissionStack; }
	
	FORCEINLINE void SetFeatherSpawnStartTime(const float InFeatherSpawnStartTime) { FeatherSpawnStartTime = InFeatherSpawnStartTime; }
	FORCEINLINE void SetDeflectCount(const int32 InDeflectCount) { DeflectCount = InDeflectCount; }
	FORCEINLINE void SetKnockBackTransmissionOffencePower(const float InKnockBackTransmissionOffencePower) { KnockBackTransmissionOffencePower = InKnockBackTransmissionOffencePower; }
	FORCEINLINE void SetKnockBackTransmissionKnockBackPower(const float InKnockBackTransmissionKnockBackPower) { KnockBackTransmissionKnockBackPower = InKnockBackTransmissionKnockBackPower; }
	FORCEINLINE void SetQuadrupleHitKnockBackPower(const float InQuadrupleHitKnockBackPower) { QuadrupleHitKnockBackPower = InQuadrupleHitKnockBackPower; }
	FORCEINLINE void SetFasterKnockBackSpeedRate(const float InFasterKnockBackRate) { FasterKnockBackSpeedRate = InFasterKnockBackRate; }
	FORCEINLINE void SetIncreaseKnockBackDamageByEnuriaCountDamageRate(const float InIncreaseKnockBackDamageByEnuriaCountDamageRate) { IncreaseKnockBackDamageByEnuriaCountDamageRate = InIncreaseKnockBackDamageByEnuriaCountDamageRate; }
	FORCEINLINE void SetRangeKnockBackKnockBackPower(const float InRangeKnockBackKnockBackPower) { RangeKnockBackKnockBackPower = InRangeKnockBackKnockBackPower; }
	FORCEINLINE void SetVampireRecoveryRate(const float InVampireRecoveryRate) { VampireRecoveryRate = InVampireRecoveryRate; }
	FORCEINLINE void SetBleedingTransmissionStack(const int32 InBleedingTransmissionStack) { BleedingTransmissionStack = InBleedingTransmissionStack; }
	
	void StartChargeFeather(float Timer);
	void AddRangeFeatherTargets(AActor* Target);
	TArray<AActor*> GetRangeFeatherTargetsAndClear();
	void VampireRecovery(float OffencePower) const;

public:
	FDissolveCompleteDelegate DissolveCompleteDelegate;
	
protected:
	void RotateToMouseCursorRecursive();
	void MoveCameraToMouseCursor();
	
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
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_AbnormalStatusAttributeSet> AbnormalStatusAttributeSet;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerCharacterAttributeSet> PlayerCharacterAttributeSet;

	// 입력 액션 관련
private:
	void MoveAction(const FInputActionValue& Value);
	void DashAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void AttackAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void SkillAction(const FInputActionValue& Value, EAbilityInputName InputName);
	void InteractAction(const FInputActionValue& Value);
	void InventoryAction(const FInputActionValue& Value);
	void PauseAction(const FInputActionValue& Value);

	// 데이터 에셋
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const ULLL_PlayerBaseDataAsset> PlayerDataAsset;

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
	float SkillCoolTime;
	uint8 bCanSkill : 1;
	uint8 bSkillRotateToMouseCursor : 1;
	FTimerHandle SkillCoolTimeTimerHandle;

	// 이누리아 관련 변수
private:
	int32 ChargedFeatherCount;
	FTimerHandle ChargeFeatherTimerHandle;
	TArray<TObjectPtr<AActor>> RangeFeatherTargets;
	float FeatherSpawnStartTime;
	float OriginKnockBackPower;
	float OriginOffencePower;
	int32 DeflectCount;
	float KnockBackTransmissionOffencePower;
	float KnockBackTransmissionKnockBackPower;
	float QuadrupleHitKnockBackPower;
	float QuadrupleHitDamageRate;
	float FasterKnockBackSpeedRate;
	float IncreaseKnockBackDamageByEnuriaCountDamageRate;
	float RangeKnockBackKnockBackPower;
	float VampireRecoveryRate;
	int32 BleedingTransmissionStack;

	// 상태 관련 함수
protected:
	void DropDissolveActor();
	void PullUpDissolveActor();
	
	UFUNCTION()
	void DeadMotionEndedHandle();
	
	// 상태 관련 변수
protected:
	uint8 bIsMoveInputPressed : 1;

	UPROPERTY()
	TObjectPtr<AActor> CharacterDissolveActor;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_PlayerGoldComponent> GoldComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_ObjectPoolingComponent> ObjectPoolingComponent;

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
