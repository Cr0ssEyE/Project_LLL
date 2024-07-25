// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "GameFramework/Actor.h"
#include "Interface/LLL_InteractiveEntityInterface.h"
#include "LLL_InteractiveObject.generated.h"

class ULLL_InteractiveObjectDataAsset;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionDelegate);

UCLASS()
class PROJECT_LLL_API ALLL_InteractiveObject : public ALLL_BaseObject, public ILLL_InteractiveEntityInterface
{
	GENERATED_BODY()
	
public:	
	ALLL_InteractiveObject();
	
	virtual void Tick(float DeltaTime) override;
	virtual void InteractiveEvent(AActor* InteractedActor = nullptr) override;

public:
	FInteractionDelegate OnInteractionDelegate;
	
protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const ULLL_InteractiveObjectDataAsset> InteractiveObjectDataAsset;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> InteractOnlyCollisionBox;
	
	UPROPERTY(EditAnywhere)
	uint8 bIsEnabled : 1;

	UPROPERTY(EditAnywhere)
	uint8 bIsOnceEventOnly : 1;
};
