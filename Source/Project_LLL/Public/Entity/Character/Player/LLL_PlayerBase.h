// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "LLL_PlayerGoldComponent.h"
#include "DataAsset/LLL_CameraDataAsset.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Interface/LLL_PlayerDependencyInterface.h"
#include "LLL_PlayerBase.generated.h"

enum class EAnimalType : uint8;
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
	
	virtual void Damaged(AActor* Attacker = nullptr, bool IsDOT = false, float Damage = 0) override;
	virtual void Dead() override;
	
	// 외부 접근용
public:
	FORCEINLINE void InitAttributeSetBySave(const FPlayerCharacterStatusData* CharacterStatusData) const { PlayerCharacterAttributeSet->InitializeSavedStatusData(CharacterStatusData); }
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
	FORCEINLINE FVector GetLastVelocityBeforeDash() const { return LastVelocityBeforeDash; };
	FORCEINLINE bool IsSkillRotateToMouseCursor() const { return bSkillRotateToMouseCursor; }
	FORCEINLINE bool CheckAttackIsRange() const { return bAttackIsRange; }
	FORCEINLINE bool CheckChargeTriggered() const { return bChargeTriggered; }
	FORCEINLINE FVector GetKnockBackDirection() const { return KnockBackDirection; }

	FORCEINLINE void SetCurrentCombo(const int32 InCurrentCombo) { CurrentCombo = InCurrentCombo; }
	FORCEINLINE void SetMoveInputPressed(const bool Press) { bIsMoveInputPressed = Press; }
	FORCEINLINE void SetSkillCoolTime(const float InSkillCoolTime) { SkillCoolTime = InSkillCoolTime; }
	FORCEINLINE void SetSkillRotateToMouseCursor(const bool SkillRotateToMouseCursor) { bSkillRotateToMouseCursor = SkillRotateToMouseCursor; }
	FORCEINLINE void SetChargeTriggered(bool ChargeTriggered) { bChargeTriggered = ChargeTriggered; }
	
	FVector CheckMouseLocation();
	void RotateToMouseCursor(float RotationMultiplyValue = 1.f, bool UseLastLocation = false);
	void StartCameraMoveToCursor(const ALLL_PlayerController* PlayerController = nullptr);
	void PauseCameraMoveToCursor() const;
	void ReadyToUseSkill();
	
	int32 GetEnuriaCount(EAnimalType AnimalType = EAnimalType::None) const;
	EAnimalType GetSkillEnuriaAnimalType() const;

public:
	// 이누리아 관련 함수
	FORCEINLINE int32 GetChargedFeatherCount() const { return ChargedFeatherCount; }
	FORCEINLINE float GetFeatherSpawnStartTime() const { return FeatherSpawnStartTime; }
	FORCEINLINE int32 GetDeflectCount() const { return DeflectCount; }
	FORCEINLINE float GetKnockBackTransmissionOffencePower() const { return KnockBackTransmissionOffencePower; }
	FORCEINLINE float GetKnockBackTransmissionKnockBackPower() const { return KnockBackTransmissionKnockBackPower; }
	FORCEINLINE float GetQuadrupleHitKnockBackPower() const { return QuadrupleHitKnockBackPower; }
	FORCEINLINE float GetFasterKnockBackSpeedRateIncrease() const { return FasterKnockBackSpeedRateIncrease; }
	FORCEINLINE float GetIncreaseKnockBackDamageDamageRateIncrease() const { return IncreaseKnockBackDamageDamageRateIncrease; }
	FORCEINLINE float GetRangeKnockBackKnockBackPower() const { return RangeKnockBackKnockBackPower; }
	FORCEINLINE int32 GetBleedingTransmissionStack() const { return BleedingTransmissionStack; }
	FORCEINLINE float GetExcessiveBleedingOffencePowerPlus() const { return ExcessiveBleedingOffencePowerPlus; }
	FORCEINLINE float GetExcessiveBleedingPeriod() const { return ExcessiveBleedingPeriod; }
	FORCEINLINE int32 GetExcessiveBleedingWolfEnuriaCheckCount() const { return ExcessiveBleedingWolfEnuriaCheckCount; }
	FORCEINLINE float GetDoubleDashDashRate() const { return DoubleDashDashRate; }
	FORCEINLINE int32 GetDoubleDashHorseEnuriaCheckCount() const { return DoubleDashHorseEnuriaCheckCount; }
	FORCEINLINE int32 GetEvasionDashHorseEnuriaCheckCount() const { return EvasionDashHorseEnuriaCheckCount; }
	FORCEINLINE float GetIncreaseKnockBackBothKnockBackPowerRate() const { return IncreaseKnockBackBothKnockBackPowerRate; }
	FORCEINLINE float GetDashAttackOffencePowerPlus() const { return DashAttackOffencePowerPlus; }
	FORCEINLINE bool CheckDashAttackCanAttack() const { return bDashAttackCanAttack; }
	FORCEINLINE float GetAttackWeakeningOffencePowerRateIncrease() const { return AttackWeakeningOffencePowerRateIncrease; }
	FORCEINLINE int32 GetAttackWeakeningWildBoarEnuriaCheckCount() const { return AttackWeakeningWildBoarEnuriaCheckCount; }
	FORCEINLINE float GetCrisisAttackMaxOffencePowerRateIncrease() const { return CrisisAttackMaxOffencePowerRateIncrease; }
	
	FORCEINLINE void SetFeatherSpawnStartTime(const float InFeatherSpawnStartTime) { FeatherSpawnStartTime = InFeatherSpawnStartTime; }
	FORCEINLINE void SetDeflectCount(const int32 InDeflectCount) { DeflectCount = InDeflectCount; }
	FORCEINLINE void SetKnockBackTransmissionOffencePower(const float InKnockBackTransmissionOffencePower) { KnockBackTransmissionOffencePower = InKnockBackTransmissionOffencePower; }
	FORCEINLINE void SetKnockBackTransmissionKnockBackPower(const float InKnockBackTransmissionKnockBackPower) { KnockBackTransmissionKnockBackPower = InKnockBackTransmissionKnockBackPower; }
	FORCEINLINE void SetQuadrupleHitKnockBackPower(const float InQuadrupleHitKnockBackPower) { QuadrupleHitKnockBackPower = InQuadrupleHitKnockBackPower; }
	FORCEINLINE void SetFasterKnockBackSpeedRateIncrease(const float InFasterKnockBackRateIncrease) { FasterKnockBackSpeedRateIncrease = InFasterKnockBackRateIncrease; }
	FORCEINLINE void SetIncreaseKnockBackDamageDamageRateIncrease(const float InIncreaseKnockBackDamageDamageRateIncrease) { IncreaseKnockBackDamageDamageRateIncrease = InIncreaseKnockBackDamageDamageRateIncrease; }
	FORCEINLINE void SetRangeKnockBackKnockBackPower(const float InRangeKnockBackKnockBackPower) { RangeKnockBackKnockBackPower = InRangeKnockBackKnockBackPower; }
	FORCEINLINE void SetVampireRecoveryRate(const float InVampireRecoveryRate) { VampireRecoveryRate = InVampireRecoveryRate; }
	FORCEINLINE void SetBleedingTransmissionStack(const int32 InBleedingTransmissionStack) { BleedingTransmissionStack = InBleedingTransmissionStack; }
	FORCEINLINE void SetExcessiveBleedingOffencePowerPlus(const float InExcessiveBleedingOffencePowerPlus) { ExcessiveBleedingOffencePowerPlus = InExcessiveBleedingOffencePowerPlus; }
	FORCEINLINE void SetExcessiveBleedingPeriod(const float InExcessiveBleedingPeriod) { ExcessiveBleedingPeriod = InExcessiveBleedingPeriod; }
	FORCEINLINE void SetExcessiveBleedingWolfEnuriaCheckCount(const int32 InExcessiveBleedingWolfEnuriaCheckCount) { ExcessiveBleedingWolfEnuriaCheckCount = InExcessiveBleedingWolfEnuriaCheckCount; }
	FORCEINLINE void SetDoubleDashDashRate(const float InDoubleDashDashRate) { DoubleDashDashRate = InDoubleDashDashRate; }
	FORCEINLINE void SetDoubleDashHorseEnuriaCheckCount(const int32 InDoubleDashHorseEnuriaCheckCount) { DoubleDashHorseEnuriaCheckCount = InDoubleDashHorseEnuriaCheckCount; }
	FORCEINLINE void SetEvasionDashHorseEnuriaCheckCount(const int32 InEvasionDashHorseEnuriaCheckCount) { EvasionDashHorseEnuriaCheckCount = InEvasionDashHorseEnuriaCheckCount; }
	FORCEINLINE void SetIncreaseKnockBackBothKnockBackPowerRate(const float InIncreaseKnockBackBothKnockBackPowerRate) { IncreaseKnockBackBothKnockBackPowerRate = InIncreaseKnockBackBothKnockBackPowerRate; }
	FORCEINLINE void SetDashAttackOffencePowerPlus(const float InDashAttackOffencePowerPlus) { DashAttackOffencePowerPlus = InDashAttackOffencePowerPlus; }
	FORCEINLINE void SetDashAttackCanAttack(const bool DashAttackCanAttack) { bDashAttackCanAttack = DashAttackCanAttack; }
	FORCEINLINE void SetAttackWeakeningOffencePowerRateIncrease(const float InAttackWeakeningOffencePowerRateIncrease) { AttackWeakeningOffencePowerRateIncrease = InAttackWeakeningOffencePowerRateIncrease; }
	FORCEINLINE void SetAttackWeakeningWildBoarEnuriaCheckCount(const int32 InAttackWeakeningWildBoarEnuriaCheckCount) { AttackWeakeningWildBoarEnuriaCheckCount = InAttackWeakeningWildBoarEnuriaCheckCount; }
	FORCEINLINE void SetCrisisAttackMaxOffencePowerRateIncrease(const float InCrisisAttackMaxOffencePowerRateIncrease) { CrisisAttackMaxOffencePowerRateIncrease = InCrisisAttackMaxOffencePowerRateIncrease; }
	
	void StartChargeFeather(float Timer);
	void AddRangeFeatherTargets(AActor* Target);
	TArray<AActor*> GetRangeFeatherTargetsAndClear();
	void VampireRecovery(float OffencePower);

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
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraComponent> AuraComponent;

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
	void AttackAction(const FInputActionValue& Value, EAbilityInputName InputName, bool Range);
	void ChargeAttackAction(const FInputActionValue& Value, EAbilityInputName InputName, bool Range);
	void ChargeAttackActionCompleted(const FInputActionValue& Value, EAbilityInputName InputName, bool Range);
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
	FVector LastVelocityBeforeDash;
	FRotator MouseDirectionRotator;
	float ToCursorRotationMultiplyValue;
	int32 LastAttackerMonsterId;
	int32 CurrentCombo;
	float SkillCoolTime;
	uint8 bCanSkill : 1;
	uint8 bSkillRotateToMouseCursor : 1;
	FTimerHandle SkillCoolTimeTimerHandle;
	uint8 bAttackIsRange : 1;
	uint8 bChargeTriggered : 1;
	FVector KnockBackDirection;

	// 이누리아 관련 변수
private:
	int32 ChargedFeatherCount;
	FTimerHandle ChargeFeatherTimerHandle;
	TArray<TObjectPtr<AActor>> RangeFeatherTargets;
	float FeatherSpawnStartTime;
	int32 DeflectCount;
	float KnockBackTransmissionOffencePower;
	float KnockBackTransmissionKnockBackPower;
	float QuadrupleHitKnockBackPower;
	float QuadrupleHitDamageRate;
	float FasterKnockBackSpeedRateIncrease;
	float IncreaseKnockBackDamageDamageRateIncrease;
	float RangeKnockBackKnockBackPower;
	float VampireRecoveryRate;
	int32 BleedingTransmissionStack;
	float ExcessiveBleedingOffencePowerPlus;
	float ExcessiveBleedingPeriod;
	int32 ExcessiveBleedingWolfEnuriaCheckCount;
	float DoubleDashDashRate;
	int32 DoubleDashHorseEnuriaCheckCount;
	int32 EvasionDashHorseEnuriaCheckCount;
	float IncreaseKnockBackBothKnockBackPowerRate;
	float DashAttackOffencePowerPlus;
	uint8 bDashAttackCanAttack : 1;
	float AttackWeakeningOffencePowerRateIncrease;
	int32 AttackWeakeningWildBoarEnuriaCheckCount;
	float CrisisAttackMaxOffencePowerRateIncrease;

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
