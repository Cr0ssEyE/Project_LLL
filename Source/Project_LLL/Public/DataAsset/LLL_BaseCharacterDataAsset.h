// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "LLL_BaseCharacterDataAsset.generated.h"

class ULLL_CharacterStatusWidget;
class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_BaseCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "스테이터스 UI")
	TSubclassOf<ULLL_CharacterStatusWidget> StatusWidgetClass;
	
	// 캐릭터 기본 모델링 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "메시", meta=(DisplayPriority=1))
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "애님 블루프린트", meta=(DisplayPriority=1))
	TSoftClassPtr<UAnimInstance> AnimInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "충돌 사이즈(높이, 반지름)", meta=(DisplayPriority=1))
	FVector2D CollisionSize;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "모델링 사이즈", meta=(DisplayPriority=1))
	FVector MeshSize = FVector::OneVector;

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "어트리뷰트 초기화 이펙트")
	TSubclassOf<UGameplayEffect> InitEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "발동형 게임플레이 어빌리티")
	TArray<TSubclassOf<UGameplayAbility>> ActiveGameplayAbility;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "수동형 게임플레이 어빌리티")
	TArray<TSubclassOf<UGameplayAbility>> PassiveGameplayAbility;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Tag", DisplayName = "발걸음 이벤트 태그")
	FGameplayTag StepCueTag;
};
