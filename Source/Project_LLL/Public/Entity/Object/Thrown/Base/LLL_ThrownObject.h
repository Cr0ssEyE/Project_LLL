// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownObjectDataAsset.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "Interface/LLL_ObjectPoolingObjectInterface.h"
#include "LLL_ThrownObject.generated.h"

class ULLL_ThrownObjectAttributeSet;
class UProjectileMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_ThrownObject : public ALLL_BaseObject, public ILLL_ObjectPoolingObjectInterface
{
	GENERATED_BODY()

public:
	ALLL_ThrownObject();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
	FORCEINLINE virtual bool IsActivated() const override { return bIsActivated; }
	
	virtual void Activate() override;
	virtual void Deactivate() override;
	
	virtual void Throw(AActor* NewOwner, const AActor* NewTarget);

protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_ThrownObjectAttributeSet> ThrownObjectAttributeSet;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_ThrownObjectDataAsset> ThrownObjectDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const AActor> Target;

	uint8 bIsActivated : 1;
};
