// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


// Object Channel
#define ECC_PLAYER ECC_GameTraceChannel11
#define ECC_ENEMY ECC_GameTraceChannel12

// Trace Channel
#define ECC_PLAYER_ONLY ECC_GameTraceChannel11
#define ECC_ENEMY_ONLY ECC_GameTraceChannel12

// Collision Profile
#define CP_STATIC_ONLY TEXT("IgnoreOnlyPawn")
#define CP_PLAYER TEXT("Player")
#define CP_MONSTER TEXT("Monster")