// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/LLL_CameraDataAsset.h"

ULLL_CameraDataAsset::ULLL_CameraDataAsset() :
	ProjectionType(ECameraProjectionMode::Orthographic),
	CameraFOV(45.f),
	SpringArmDistance(3000.f),
	bUseConstraintAspectRatio(false),
	AspectRatio(1.77778f),
	CameraDistance(3000.f),
	SpringArmAngle(-45.f, -45.f, 0.f),
	bUseAutoCalculate(false),
	AutoPlaneShift(0),
	bUseUpdateOrthoPlanes(false),
	OrthographicNearClipDistance(-3000),
	OrthographicFarClipDistance(10000000),
	CameraCursorTrackingSpeed(500),
	CameraCursorTrackingLength(0)
{
}
