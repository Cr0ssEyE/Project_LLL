// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownBleedingDataAsset.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "LLL_ThrownBleeding.generated.h"

class ALLL_MonsterBase;
class ULLL_ThrownBleedingAttributeSet;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_ThrownBleeding : public ALLL_ThrownObject
{
	GENERATED_BODY()

public:
	ALLL_ThrownBleeding();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	
	virtual void Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override {}
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> OverlapCollisionBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ThrownBleedingDataAsset> ThrownBleedingDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_ThrownBleedingAttributeSet> ThrownBleedingAttributeSet;

	TArray<TObjectPtr<ALLL_MonsterBase>> DamagedMonsters;
	float BleedingOffencePower;
};
