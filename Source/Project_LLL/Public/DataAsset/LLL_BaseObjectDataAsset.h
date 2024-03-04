// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_BaseObjectDataAsset.generated.h"

class UGameplayEffect;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_BaseObjectDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	ULLL_BaseObjectDataAsset();
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh", DisplayName = "스태틱 메시", meta=(EditCondition = "bIsUsingSkeletalMesh == false", EditConditionHides))
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh", DisplayName = "스켈레탈 메시", meta=(EditCondition = "bIsUsingSkeletalMesh == true", EditConditionHides))
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "어트리뷰트 초기화 이펙트")
	TSubclassOf<UGameplayEffect> InitEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "발동형 게임플레이 어빌리티")
	TArray<TSubclassOf<UGameplayAbility>> ActiveGameplayAbility;

	// 코드상에서 메시 타입 선택을 강제할지 알아서 선택할 수 있게 할지 고민...
	// 작성자 개인 의견으로는 코드에서 강제하는 편이 낫다 생각합니다.
	UPROPERTY(VisibleAnywhere, Category = "StaticMesh", DisplayName = "스켈레탈 메시 사용")
	uint32 bIsUsingSkeletalMesh : 1;
	
};
