// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_PlayerThrownFeatherDataAsset.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "LLL_PlayerThrownFeather.generated.h"

class UBoxComponent;
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
	virtual void Activate() override;
	virtual void Deactivate() override;

	UPROPERTY(VisibleAnywhere)
	float CurveSpeed;

	UPROPERTY(EditAnywhere)
	float CurveSize;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitCollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_PlayerThrownFeatherDataAsset> PlayerThrownFeatherDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerThrownFeatherAttributeSet> PlayerThrownFeatherAttributeSet;
};
