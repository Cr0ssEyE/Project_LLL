// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownObjectDataAsset.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "Interface/LLL_ObjectPoolingObjectInterface.h"
#include "LLL_ThrownObject.generated.h"

class ALLL_BaseCharacter;
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

	FORCEINLINE void SetAbilityInfo(const FAbilityDataTable* InAbilityData, float InAbilityLevel) { AbilityData = InAbilityData; AbilityLevel = InAbilityLevel; }
	
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }
	FORCEINLINE virtual bool IsActivated() const override { return bIsActivated; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Activate() override;
	virtual void Deactivate() override;

public:
	virtual void Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower);

protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void DamageTo(AActor* OtherActor);
	
	void KnockBackTo(const FVector& Direction, AActor* OtherActor) const;
	
	UFUNCTION()
	void TargetDeadHandle(ALLL_BaseCharacter* Character);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_ThrownObjectAttributeSet> ThrownObjectAttributeSet;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_ThrownObjectDataAsset> ThrownObjectDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> Target;

	uint8 bIsActivated : 1;
	
	FTimerHandle HideTimerHandle;
	const FAbilityDataTable* AbilityData;
	float AbilityLevel;
	float OffencePower;

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
	
	UPROPERTY(VisibleAnywhere)
	uint8 bIsStraight : 1;

	UPROPERTY(VisibleAnywhere)
	float KnockBackPower;
};
