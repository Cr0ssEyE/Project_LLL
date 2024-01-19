// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/LLL_ObjectBase.h"
#include "LLL_ThrownObject.generated.h"

class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_ThrownObject : public ALLL_ObjectBase
{
	GENERATED_BODY()

public:
	ALLL_ThrownObject();

	FORCEINLINE float GetSpeed() const { return Speed; }
	
	void Throw(AActor* NewOwner);

private:
	UFUNCTION()
	void HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
	float Speed;

	UPROPERTY(VisibleAnywhere)
	float OffensePower;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};
