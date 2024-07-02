// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownFeatherDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "LLL_ThrownFeather.generated.h"

class UBoxComponent;
class ULLL_ThrownFeatherAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_ThrownFeather : public ALLL_ThrownObject
{
	GENERATED_BODY()

public:
	ALLL_ThrownFeather();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	virtual void Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed) override;

	UFUNCTION()
	void TargetDeadHandle(ALLL_BaseCharacter* Character);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitCollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_ThrownFeatherDataAsset> ThrownFeatherDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_ThrownFeatherAttributeSet> ThrownFeatherAttributeSet;

	UPROPERTY(VisibleAnywhere)
	float CurveSpeed;

	UPROPERTY(VisibleAnywhere)
	float CurveSize;

	UPROPERTY(VisibleAnywhere)
	float TargetCapsuleRadius;

	UPROPERTY(VisibleAnywhere)
	FVector TargetDeadLocation;

	UPROPERTY(VisibleAnywhere)
	uint8 bTargetIsDead : 1;
};
