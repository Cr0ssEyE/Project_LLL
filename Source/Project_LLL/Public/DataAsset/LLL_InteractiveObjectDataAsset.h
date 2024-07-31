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
};
