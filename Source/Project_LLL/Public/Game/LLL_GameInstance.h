// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_GameInstance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_LLL_API ULLL_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ULLL_GameInstance();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> PlayerMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> ObjectMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> MonsterMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> InterfaceMPC;
};
