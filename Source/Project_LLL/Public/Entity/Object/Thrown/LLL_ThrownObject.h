// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownObjectData.h"
#include "Entity/Object/LLL_BaseObject.h"
#include "LLL_ThrownObject.generated.h"

class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_ThrownObject : public ALLL_BaseObject
{
	GENERATED_BODY()

public:
	ALLL_ThrownObject();

	FORCEINLINE float GetSpeed() const { return Speed; }
	
	void Throw(AActor* NewOwner);

protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	float Speed;

	UPROPERTY(VisibleAnywhere)
	float OffensePower;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_ThrownObjectData> ThrownObjectData;
};
