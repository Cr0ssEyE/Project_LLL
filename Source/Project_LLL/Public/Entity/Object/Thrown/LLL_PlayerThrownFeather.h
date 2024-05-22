// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "LLL_PlayerThrownFeather.generated.h"

class ULLL_PlayerThrownFeatherAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_PlayerThrownFeather : public ALLL_ThrownObject
{
	GENERATED_BODY()

public:
	ALLL_PlayerThrownFeather();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Deactivate() override;

	float CurrentCurveSize;

	UPROPERTY(EditAnywhere)
	float CurveSize;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerThrownFeatherAttributeSet> PlayerThrownFeatherAttributeSet;
};
