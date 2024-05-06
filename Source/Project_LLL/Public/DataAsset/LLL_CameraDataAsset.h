// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_CameraDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CameraDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	ULLL_CameraDataAsset();
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 투영 종류")
	TEnumAsByte<ECameraProjectionMode::Type> ProjectionType;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 FOV")
	float CameraFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 거리")
	float CameraDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 회전값")
	FRotator SpringArmAngle;
};
