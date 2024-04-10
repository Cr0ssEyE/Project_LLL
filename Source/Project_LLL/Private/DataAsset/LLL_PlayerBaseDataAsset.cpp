// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/LLL_PlayerBaseDataAsset.h"

ULLL_PlayerBaseDataAsset::ULLL_PlayerBaseDataAsset()
{
	CameraFOV = 40.f;
	SpringArmLength = 3000.f;
	SpringArmAngle = FRotator(0.f, -45.f, -45.f);
}
