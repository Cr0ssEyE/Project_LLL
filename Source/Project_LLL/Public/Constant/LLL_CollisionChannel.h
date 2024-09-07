// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


// 오브젝트 채널. 1~
#define ECC_PLAYER ECC_GameTraceChannel1 // Player
#define ECC_ENEMY ECC_GameTraceChannel2 // Monster
#define ECC_GIMMICK ECC_EngineTraceChannel3 // Gimmick
#define ECC_FALLOUT_WALL ECC_GameTraceChannel4 // FallOutableWall

// 트레이스 채널. 11~
#define ECC_PLAYER_HIT ECC_GameTraceChannel11 // TracePlayerHit
#define ECC_ENEMY_HIT ECC_GameTraceChannel12 // TraceNonPlayerHit
#define ECC_PLAYER_CHECK ECC_GameTraceChannel13 // TracePlayerOverlap
#define ECC_ENTITY_CHECK ECC_GameTraceChannel14 // TraceNonPlayerOverlap
#define ECC_WALL_ONLY ECC_GameTraceChannel15 // TraceStaticOnly
#define ECC_TRACE_FIELD ECC_GameTraceChannel16 // TraceField

// 콜리전 프로필
#define CP_PLAYER_ABILITY_OBJECT TEXT("PlayerAbilityObject")
#define CP_PLAYER_THROWN_OBJECT TEXT("PlayerThrownObject")
#define CP_NO_COLLISION TEXT("NoCollision")
#define CP_OVERLAP_ALL TEXT("OverlapAll")
#define CP_INVISIBLE_WALL TEXT("InvisibleWall")
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
#define CP_MONSTER_ABILITY_OBJECT TEXT("MonsterAbilityObject")

// 감지용 콜리전 프로필
#define CP_BULLET_TIME_INFLUENCED TEXT("BulletTimeInfluencedActor")
#define CP_MAP_SOUND_MANAGER TEXT("MapSoundManager")
