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
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 FOV", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Perspective", EditConditionHides))
	float CameraFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 암 거리")
	float SpringArmDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 거리", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float CameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "근평면 클리핑 거리", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float OrthographicNearClipDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "원평면 클리핑 거리", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float OrthographicFarClipDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 회전값")
	FRotator SpringArmAngle;
};
