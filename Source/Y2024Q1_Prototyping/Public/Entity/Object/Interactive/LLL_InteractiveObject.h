// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/LLL_ObjectBase.h"
#include "GameFramework/Actor.h"
#include "Interface/LLL_InteractiveEntityInterface.h"
#include "LLL_InteractiveObject.generated.h"

UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_InteractiveObject : public ALLL_ObjectBase, public ILLL_InteractiveEntityInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALLL_InteractiveObject();
	
	virtual void Tick(float DeltaTime) override;
	virtual void InteractiveEvent() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> InteractOnlyCollisionBox;
	
	UPROPERTY()
	uint8 bIsEnabled : 1;

	UPROPERTY()
	uint8 bIsOnceEventOnly : 1;
};
