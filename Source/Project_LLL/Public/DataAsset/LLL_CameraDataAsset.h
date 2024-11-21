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

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 암 거리", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Perspective", EditConditionHides))
	float PerspectiveSpringArmDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "고정 종횡비 사용", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	uint8 bUseConstraintAspectRatio : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "종횡비 비율", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic && bUseConstraintAspectRatio == true", EditConditionHides))
	float AspectRatio;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 거리(직교 너비)", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float CameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 거리(직교 너비)", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float CameraDistanceWhenBossExist;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 회전값")
	FRotator SpringArmAngle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|Plane", DisplayName = "평면 거리 자동 조절 사용", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	uint8 bUseAutoCalculate : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Plane", DisplayName = "근|원평면 자동 거리 조정", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic && bUseAutoCalculate == true", EditConditionHides))
	float AutoPlaneShift;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|Plane", DisplayName = "평면 거리 업데이트 사용", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	uint8 bUseUpdateOrthoPlanes : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Plane", DisplayName = "근평면 클리핑 거리", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float OrthographicNearClipDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Plane", DisplayName = "원평면 클리핑 거리", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float OrthographicFarClipDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera", DisplayName = "카메라 암 거리", meta=(EditCondition = "ProjectionType == ECameraProjectionMode::Orthographic", EditConditionHides))
	float OrthographicSpringArmDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera|Plane", DisplayName = "카메라가 마우스를 따라가는 속도")
	float CameraCursorTrackingSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Plane", DisplayName = "카메라가 마우스를 따라가는 최대 거리(0인 경우 속도 = 최대 거리)")
	float CameraCursorTrackingLength;
};
