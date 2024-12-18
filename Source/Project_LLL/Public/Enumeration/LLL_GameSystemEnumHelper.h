﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 보상 및 맵 시스템 관련
UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT,
	BURST
};

UENUM()
enum class EProductType : uint8
{
	Gold = 1,
	MaxHP,
	Ability
};

UENUM()
enum class ERewardCategory : uint8
{
	Gold = 1,
	Ability,
	Enhance,
	MaxHP
};

// 로비 관련
UENUM(BlueprintType)
enum class ELobbyCustomPointType : uint8
{
	InteractionObject,
	InteractionCharacter,
	Housing,
	EnumEnd UMETA(Hidden)
};



// FMOD 관련
UENUM(BlueprintType)
enum class EFModParameter : uint8
{
	None,
	PlayerWalkMaterialParameter,
	MonsterWalkMaterialParameter,
	PlayerAttackCountParameter,
	PlayerAttackHitCountParameter,
	PlayerDamagedTypeParameter,
	BGM_BulletTimeParameter,
	BGM_BattleParameter,
	BGM_PauseParameter,
	AMB_BulletTimeParameter
};

UENUM(BlueprintType)
enum class EPlayerWalkMaterialParameter : uint8
{
	Dirt,
	Grass,
	Wood,
	Steel
};

UENUM(BlueprintType)
enum class EMonsterWalkMaterialParameter : uint8
{
	Dirt,
	Grass,
	Wood,
	Steel
};

UENUM(BlueprintType)
enum class EPlayerDamagedTypeParameter : uint8
{
	Sword,
	Claw,
	Staff
};