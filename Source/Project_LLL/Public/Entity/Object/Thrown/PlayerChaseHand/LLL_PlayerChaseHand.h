// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "Interface/LLL_PlayerDependencyInterface.h"
#include "LLL_PlayerChaseHand.generated.h"

class USphereComponent;
class ULLL_PlayerChaseHandAttributeSet;
class ULLL_PlayerChaseHandDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHandMovementCompleteDelegate);

UCLASS()
class PROJECT_LLL_API ALLL_PlayerChaseHand : public ALLL_ThrownObject, public ILLL_PlayerDependencyActorInterface
{
	GENERATED_BODY()

public:
	ALLL_PlayerChaseHand();

public:
	FHandMovementCompleteDelegate ThrowCompleteDelegate;
	FHandMovementCompleteDelegate OnGrabbedDelegate;
	FHandMovementCompleteDelegate ReleaseCompleteDelegate;

public:
	FORCEINLINE USphereComponent* GetCollisionComponent() const { return HandCollision; }
	FORCEINLINE USkeletalMeshComponent* GetHandMesh() const { return HandMesh; }
	FORCEINLINE AActor* GetGrabbedActor() const { return GrabbedActor; }
	
public:
	FORCEINLINE void SetGrabbedActor(AActor* Actor) { GrabbedActor = Actor; }
	
	void SetHiddenState();
	
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// 기본 컴포넌트 및 객체
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HandMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> HandCollision;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> GrabbedActor;
	
protected:
	UPROPERTY()
	uint32 bIsGrabbed : 1;
};
