// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownFeatherDataAsset.h"
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

	FORCEINLINE float GetChaseFeatherThrowAngleOffset() const { return ChaseFeatherThrowAngleOffset; }
	FORCEINLINE int32 GetChaseFeatherThrowAngleRandomMultiply() const { return ChaseFeatherThrowAngleRandomMultiply; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	
	virtual void Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void DamageTo(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> HitCollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_ThrownFeatherDataAsset> ThrownFeatherDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_ThrownFeatherAttributeSet> ThrownFeatherAttributeSet;
	
	float ChaseFeatherThrowAngleOffset;
	int32 ChaseFeatherThrowAngleRandomMultiply;
};
