// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseObjectDataAsset.h"
#include "LLL_InteractiveObjectDataAsset.generated.h"

class UFMODEvent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_InteractiveObjectDataAsset : public ULLL_BaseObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "상호작용 사운드 이벤트")
	TObjectPtr<UFMODEvent> ActivateEvent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Collision", DisplayName = "상호작용 콜리젼 사이즈")
	FVector InteractOnlyCollisionBoxExtent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction", DisplayName = "상호작용 UI 커스텀 표기 사용(추후 테이블 이전 예정)")
	uint8 bUseCustomDisplayName : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction", DisplayName = "상호작용 UI 커스텀 표기 명(추후 테이블 이전 예정)", meta = (EditCondition = "bUseCustomDisplayName == true", EditConditionHides))
	FString CustomDisplayTextString;
	
};
