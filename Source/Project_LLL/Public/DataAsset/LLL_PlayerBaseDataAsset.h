// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterDataAsset.h"
#include "LLL_PlayerBaseDataAsset.generated.h"

enum class EPlayerDamagedTypeParameter : uint8;
enum class EPlayerWalkMaterialParameter : uint8;
enum class EAbilityInputName : uint8;
class UNiagaraSystem;
class UInputAction;
class UInputMappingContext;
class ULevelSequence;
class ULLL_MainEruriaInfoWidget;
class ULLL_SelectRewardWidget;
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

public:
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "허리 부착 파티클")
	TObjectPtr<UNiagaraSystem> SpineParticle;
	
	// UI 관련 
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "일시정지 UI")
	TSubclassOf<ULLL_GamePauseWidget> GamePauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "인벤토리 UI")
	TSubclassOf<ULLL_InventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "상호작용 정보 UI")
	TSubclassOf<ULLL_InteractionWidget> InteractionWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "보상 선택 UI")
	TSubclassOf<ULLL_SelectRewardWidget> SelectRewardWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "메인 이누리아 UI")
	TSubclassOf<ULLL_MainEruriaInfoWidget> MainEruriaInfoWidgetClass;
	
	// 애니메이션 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "캐릭터 사망 연출 시퀀서")
	TObjectPtr<ULevelSequence> DeadSequencer;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "캐릭터 사망 연출용 디졸브 액터")
	TSubclassOf<AActor> CharacterDissolveActor;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "디졸브 액터 낙하 속도")
	float DissolveActorFallSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "디졸브 액터 정지 위치(플레이어 위치 기준)")
	float DissolveActorFallStopLocation;
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "보조 액션 입력 키")
	TObjectPtr<UInputAction> ControlChaseInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "상호작용 입력 키")
	TObjectPtr<UInputAction> InteractionInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "인벤토리 입력 키")
	TObjectPtr<UInputAction> InventoryInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "일시정지 입력 키")
	TObjectPtr<UInputAction> PauseInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input", DisplayName = "스킬 입력 키")
	TObjectPtr<UInputAction> SkillInputAction;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "발걸음 이벤트 파라미터 속성")
	TMap<TEnumAsByte<EPhysicalSurface>, EPlayerWalkMaterialParameter> StepEventParameterProperties;

	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "피격 이벤트 파라미터 속성")
	TMap<int32, EPlayerDamagedTypeParameter> DamagedEventParameterProperties;

public:
	UPROPERTY(EditDefaultsOnly, Category = "PP", DisplayName = "PP HPLow 파라미터 최대값")
	float HPLowScalarMaxValue;

	UPROPERTY(EditDefaultsOnly, Category = "PP", DisplayName = "PP HPLow 파라미터 최소값")
	float HPLowScalarLowValue;

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "넉백된 대상이 받는 데미지 이펙트")
	TSubclassOf<UGameplayEffect> KnockBackCauserDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "넉백된 대상과 충돌한 대상이 받는 데미지 이펙트")
	TSubclassOf<UGameplayEffect> KnockBackTargetDamageEffect;

	// 이누리아 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Enuria", DisplayName = "연쇄 작용 이누리아 데미지 이펙트")
	TSubclassOf<UGameplayEffect> KnockBackTransmissionDamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Enuria", DisplayName = "맹렬한 공세 이누리아 데미지 타임 오프셋")
	float QuadrupleHitHitOffsetTime;

	UPROPERTY(EditDefaultsOnly, Category = "Enuria", DisplayName = "피의 갈증 이누리아 회복 이펙트")
	TSubclassOf<UGameplayEffect> VampireRecoveryEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enuria", DisplayName = "피의 역병 이누리아 데미지 이펙트")
	TSubclassOf<UGameplayEffect> BleedingTransmissionDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enuria", DisplayName = "혈우병 이누리아 데미지 이펙트")
	TSubclassOf<UGameplayEffect> BleedingExplosionDamageEffect;
};
