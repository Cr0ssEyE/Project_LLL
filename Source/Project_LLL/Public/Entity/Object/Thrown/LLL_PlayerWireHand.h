// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_ThrownObject.h"
#include "LLL_PlayerWireHand.generated.h"

class USphereComponent;
class ULLL_PlayerWireHandAttributeSet;
class ULLL_PlayerWireObjectDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWireMovementCompleteDelegate);

UCLASS()
class PROJECT_LLL_API ALLL_PlayerWireHand : public ALLL_ThrownObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_PlayerWireHand();

	FORCEINLINE ULLL_PlayerWireHandAttributeSet* GetWireHandAttributeSet() const { return WireHandAttributeSet; }
	FORCEINLINE USphereComponent* GetCollisionComponent() { return HandCollision; }
	FORCEINLINE USkeletalMeshComponent* GetHandMesh() { return HandMesh; }
	
	void SetNormalState();
	void SetThrowState(const FVector Location);
	void SetReleaseState(const FVector Location);

	FWireMovementCompleteDelegate ThrowCompleteDelegate;
	FWireMovementCompleteDelegate OnGrabbedDelegate;
	FWireMovementCompleteDelegate ReleaseCompleteDelegate;
	
protected:
	virtual void PostInitializeComponents() override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	// 기본 컴포넌트 및 객체
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HandMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> HandCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerWireHandAttributeSet> WireHandAttributeSet;

protected:
	void CheckReached();
	
	void RetargetReleaseVelocity();

protected:
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	float CorrectionReachStateDistance;

	UPROPERTY()
	uint32 bIsGrabbed : 1;
};
