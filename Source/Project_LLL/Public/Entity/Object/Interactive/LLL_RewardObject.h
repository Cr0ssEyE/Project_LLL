// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
#include "LLL_RewardObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_RewardObject : public ALLL_InteractiveObject
{
	GENERATED_BODY()
	
public:
	ALLL_RewardObject();

	virtual void InteractiveEvent() override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> RewardMesh;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class ULLL_SelectRewardWidget> SelectRewardWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ULLL_SelectRewardWidget> SelectRewardWidget;
};
