// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BossMonsterPatternEnumHelper.generated.h"

UENUM(BlueprintType)
enum class EBossMonsterPattern : uint8
{
	ManOfStrength_Shockwave,
	ManOfStrength_Dash,
	ManOfStrength_ApneaInAttack,
	ManOfStrength_SnapOtherMonster
};