// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ESelectShapeTypes : uint8
{
	Box,
	Capsule,
	Sphere,
	Cone
};

UENUM(BlueprintType)
enum class ESelectTraceTarget : uint8
{
	Player UMETA(DisplayName="Player"),
	Monster UMETA(DisplayName="Monster"),
	AllEntity UMETA(DisplayName="AllEntity")
};

/*
 *	임시 데이터
 */
UENUM(BlueprintType)
enum class ELevelSequenceType : uint8
{
	EnumStart = 0 UMETA(Hidden),
	Lobby_Intro,
	StageOne_Boss_Intro,
	StageOne_Boss_Defeat,
	StageTwo_Boss_Intro,
	StageTwo_Boss_Defeat,
	StageThree_Boss_Intro,
	StageThree_Boss_Defeat,
	EnumEnd UMETA(Hidden)
};