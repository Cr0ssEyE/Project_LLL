// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_PlayerBaseDataAsset.generated.h"

class UInputAction;
class UInputMappingContext;
class ULLL_PlayerAnimInstance;

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerBaseDataAsset : public UDataAsset
{
	GENERATED_BODY()

	// 캐릭터 기본 설정
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "플레이어 캐릭터 메시")
	TObjectPtr<USkeletalMesh> PlayerBaseMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "플레이어 캐릭터 애님 블루프린트")
	TSubclassOf<ULLL_PlayerAnimInstance> PlayerAnimBlueprint;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "플레이어 캐릭터 충돌 사이즈(높이, 반지름)")
	FVector2D PlayerCollisionSize;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 FOV")
	float CameraFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 거리")
	float SpringArmLength;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 기울기")
	float SpringArmAngle;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "키 입력 매핑 IMC")
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "이동 입력 키")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "회피 입력 키")
	TObjectPtr<UInputAction> EvadeInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "기본 공격 입력 키")
	TObjectPtr<UInputAction> AttackInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "스킬 입력 키")
	TObjectPtr<UInputAction> SkillInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "상호작용 입력 키")
	TObjectPtr<UInputAction> InteractionInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "인벤토리 입력 키")
	TObjectPtr<UInputAction> InventoryInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "일시정지 입력 키")
	TObjectPtr<UInputAction> PauseInputAction;
};
