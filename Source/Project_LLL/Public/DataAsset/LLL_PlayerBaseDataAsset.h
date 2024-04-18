// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterDataAsset.h"
#include "LLL_PlayerBaseDataAsset.generated.h"

class UFMODEvent;
enum class ESFX_Player_Walk_Material : uint8;
enum class EPlayerFootstepsSurface : uint8;
class UInputAction;
class UInputMappingContext;
enum class EAbilityInputName;
class ULLL_SkillWidget;
class ULLL_InteractionWidget;
class ULLL_InventoryWidget;
class ULLL_GamePauseWidget;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerBaseDataAsset : public ULLL_BaseCharacterDataAsset
{
	GENERATED_BODY()
	
	// UI 관련 
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "일시정지 UI")
	TSubclassOf<ULLL_GamePauseWidget> GamePauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "인벤토리 UI")
	TSubclassOf<ULLL_InventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "상호작용 정보 UI")
	TSubclassOf<ULLL_InteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "스킬 게이지 UI")
	TSubclassOf<ULLL_SkillWidget> SkillGaugeWidgetClass;

	// 애니메이션 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "캐릭터 사망 애님 몽타주")
	TObjectPtr<UAnimMontage> DeadAnimMontage;

	// 상호작용 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Action", DisplayName = "마우스 입력시 위치 보정 처리 영역 반지름")
	float MouseCursorCorrectRadius;
	
	// 입력 이벤트 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "입력 어빌리티")
	TMap<EAbilityInputName, TSubclassOf<UGameplayAbility>> DefaultSkillAbility;

	// 입력 설정 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "키 입력 매핑 IMC")
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "이동 입력 키")
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "돌진 입력 키")
	TObjectPtr<UInputAction> DashInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "기본 공격 입력 키")
	TObjectPtr<UInputAction> AttackInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "와이어 액션 입력 키")
	TObjectPtr<UInputAction> ControlWireInputAction;
	
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
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "발걸음 이벤트 파라미터 이름")
	FName StepEventParameterName;
	
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "발걸음 이벤트 파라미터 속성")
	TMap<TEnumAsByte<EPhysicalSurface>, ESFX_Player_Walk_Material> StepEventParameterProperties;

	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "스테이지 1 배경음")
	TObjectPtr<UFMODEvent> Stage1AMB;
};
