﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_FallableWallGimmick.generated.h"

class ALLL_MonsterBase;

UCLASS()
class PROJECT_LLL_API ALLL_FallableWallGimmick : public ALLL_SystemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_FallableWallGimmick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
protected:
	bool CheckFallable(FVector HitNormal, FVector HitLocation);
	
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Wall;

	float FallRequiredVelocityLength;
};
