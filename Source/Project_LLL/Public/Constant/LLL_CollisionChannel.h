// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


// Object Channel
#define ECC_PLAYER ECC_GameTraceChannel1
#define ECC_ENEMY ECC_GameTraceChannel2
#define ECC_GIMMICK ECC_EngineTraceChannel3
#define ECC_FALLOUT_WALL ECC_GameTraceChannel4

// Trace Channel
#define ECC_PLAYER_HIT ECC_GameTraceChannel11
#define ECC_ENEMY_HIT ECC_GameTraceChannel12
#define ECC_PLAYER_CHECK ECC_GameTraceChannel13
#define ECC_ENTITY_CHECK ECC_GameTraceChannel14
#define ECC_WALL_ONLY ECC_GameTraceChannel15
#define ECC_TRACE_FIELD ECC_GameTraceChannel16

// Collision Profile
#define CP_NO_COLLISION TEXT("NoCollision")
#define CP_OVERLAP_ALL TEXT("OverlapAll")
#define CP_STATIC_ONLY TEXT("IgnoreOnlyPawn")
#define CP_RAGDOLL TEXT("Ragdoll")
#define CP_PLAYER TEXT("Player")
#define CP_PLAYER_EVADE TEXT("Player_Evade")
#define CP_PLAYER_CHASE_HAND TEXT("Player_ChaseHand")
#define CP_MONSTER TEXT("Monster")
#define CP_MONSTER_FALLABLE TEXT("Monster_Fallable")
#define CP_MONSTER_ATTACK TEXT("MonsterAttack")
#define CP_MONSTER_DASH TEXT("MonsterDash")
#define CP_INTERACTION TEXT("Interaction")
#define CP_STAGE_TRIGGER TEXT("StageTrigger")
#define CP_BULLET_TIME_INFLUENCED TEXT("BulletTimeInfluencedActor")
