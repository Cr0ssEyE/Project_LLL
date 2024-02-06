// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterDataAsset.h"
#include "LLL_PlayerBaseDataAsset.generated.h"

class ULLL_WeaponBaseDataAsset;
class ULLL_InteractionWidget;
class ULLL_InventoryWidget;
class ULLL_PlayerStatusWidget;
class ULLL_GamePauseWidget;
class ULLL_PlayerAnimInstance;
class UInputAction;
class UInputMappingContext;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerBaseDataAsset : public ULLL_BaseCharacterDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SubData", DisplayName = "캐릭터 기본 무기 데이터")
	TObjectPtr<ULLL_WeaponBaseDataAsset> DefaultWeaponBaseDataAsset;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "일시정지 UI")
	TSubclassOf<ULLL_GamePauseWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "인벤토리 UI")
	TSubclassOf<ULLL_InventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "상호작용 정보 UI")
	TSubclassOf<ULLL_InteractionWidget> InteractionWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "스테이터스 UI")
	TSubclassOf<ULLL_PlayerStatusWidget> StatusWidgetClass;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 FOV")
	float CameraFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 거리")
	float SpringArmLength;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 기울기")
	float SpringArmAngle;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "기본 돌진속도")
	float DashSpeed;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action", DisplayName = "돌진 기본 사용 횟수")
	uint32 DashBaseCount;

	UPROPERTY(EditDefaultsOnly, Category = "Action", DisplayName = "돌진 연속사용 입력체크 유지 시간")
	float DashInputCheckTime;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action", DisplayName = "돌진 재사용 대기 시간")
	float DashBaseCoolDownSeconds;

	UPROPERTY(EditDefaultsOnly, Category = "Action", DisplayName = "돌진 기본 무적 시간")
	float DashBaseInvincibleTime;

	UPROPERTY(EditDefaultsOnly, Category = "Action", DisplayName = "상호작용 거리")
	float InteractionRange;

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "기본 스킬")
	TMap<int32, TSubclassOf<UGameplayAbility>> DefaultSkillAbility;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "키 입력 매핑 IMC")
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "이동 입력 키")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "돌진 입력 키")
	TObjectPtr<UInputAction> DashInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "기본 공격 입력 키")
	TObjectPtr<UInputAction> AttackInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "스킬 입력 키")
	TObjectPtr<UInputAction> SkillInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "상호작용 입력 키")
	TObjectPtr<UInputAction> InteractionInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "상호작용 대상 전환 키")
	TObjectPtr<UInputAction> InteractiveTargetChangeInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "인벤토리 입력 키")
	TObjectPtr<UInputAction> InventoryInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "일시정지 입력 키")
	TObjectPtr<UInputAction> PauseInputAction;
};
