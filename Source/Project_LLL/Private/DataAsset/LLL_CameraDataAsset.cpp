// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/LLL_CameraDataAsset.h"

ULLL_CameraDataAsset::ULLL_CameraDataAsset()
{
	CameraFOV = 40.f;
	CameraDistance = 3000.f;
	SpringArmAngle = FRotator(-45.f, -45.f, 0.f);
}
