// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_ThrownObject.h"

#include "LLL_PlayerWireHand.generated.h"


class ULLL_PlayerWireHandAttributeSet;
class ULLL_PlayerWireObjectDataAsset;

UCLASS()
class PROJECT_LLL_API ALLL_PlayerWireHand : public ALLL_ThrownObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_PlayerWireHand();

	FORCEINLINE ULLL_PlayerWireHandAttributeSet* GetWireHandAttributeSet() const {return WireHandAttributeSet; }
	
protected:
	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	// Called every frame
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HandMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerWireHandAttributeSet> WireHandAttributeSet;
};
