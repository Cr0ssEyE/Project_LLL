// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_CharacterBaseDataAsset.h"
#include "LLL_PlayerBaseDataAsset.generated.h"

class ULLL_InteractionWidget;
class ULLL_InventoryWidget;
class ULLL_PlayerStatusWidget;
class ULLL_GamePauseWidget;
class UInputAction;
class UInputMappingContext;
class ULLL_PlayerAnimInstance;

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerBaseDataAsset : public ULLL_CharacterBaseDataAsset
{
	GENERATED_BODY()

	// 캐릭터 기본 설정
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "플레이어 캐릭터 공격 애님 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "플레이어 캐릭터 충돌 사이즈(높이, 반지름)")
	FVector2D PlayerCollisionSize;

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
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "플레이어 기본 이동속도")
	float PlayerBaseMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "플레이어 기본 가속도")
	float PlayerBaseAccelerateSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "플레이어 기본 돌진속도")
	float PlayerBaseDashSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "플레이어 방향전환 속도")
	float PlayerBaseTurnSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "플레이어 지면 마찰력")
	float PlayerBaseGroundFriction;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 FOV")
	float CameraFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 거리")
	float SpringArmLength;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 기울기")
	float SpringArmAngle;

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
