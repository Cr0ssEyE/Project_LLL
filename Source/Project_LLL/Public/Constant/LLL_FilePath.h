// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define PATH_PLAYER_EFFECTS TEXT("PlayerEffects")
#define PATH_MONSTER_EFFECTS TEXT("MonsterEffects")
#define PATH_OBJECT_EFFECTS TEXT("ObjectEffects")
#define PATH_SHARE_EFFECTS TEXT("ShareEffects")

// 플레이어 관련
#define PATH_PLAYER_DATA TEXT("/Script/Project_LLL.LLL_PlayerBaseDataAsset'/Game/DataAsset/DA_Player.DA_Player'")
#define PATH_PLAYER_CHASE_HAND_DATA TEXT("/Script/Project_LLL.LLL_PlayerChaseHandDataAsset'/Game/DataAsset/DA_PlayerChaseHand.DA_PlayerChaseHand'")
#define PATH_CAMERA_DATA TEXT("/Script/Project_LLL.LLL_CameraDataAsset'/Game/DataAsset/DA_Camera.DA_Camera'")

// 몬스터 캐릭터 관련
#define PATH_ORAWAVE_DATA TEXT("/Script/Project_LLL.LLL_OrawaveDataAsset'/Game/DataAsset/DA_Orawave.DA_Orawave'")
#define PATH_ENERGY_BLASTER_DATA TEXT("/Script/Project_LLL.LLL_EnergyBlasterDataAsset'/Game/DataAsset/DA_EnergyBlaster.DA_EnergyBlaster'")
#define PATH_ENERGY_BLASTER_ENERGY_DATA TEXT("/Script/Project_LLL.LLL_EnergyBlasterEnergyDataAsset'/Game/DataAsset/DA_EnergyBlasterEnergy.DA_EnergyBlasterEnergy'")
#define PATH_SWORD_DASH_DATA TEXT("/Script/Project_LLL.LLL_SwordDashDataAsset'/Game/DataAsset/DA_SwordDash.DA_SwordDash'")

// 오브젝트 관련
#define PATH_INTERACTIVE_OBJECT_DATA TEXT("/Script/Project_LLL.LLL_InteractiveObjectData'/Game/DataAsset/DA_InteractiveObject.DA_InteractiveObject'")
#define PATH_BREAKABLE_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'")
#define PATH_BREAKABLE_OBJECT_TEST_EFFECT TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/DropGold/BPGE_DropGold.BPGE_DropGold_C'")

// 시스템 관련
#define PATH_MONSTER_SPAWNER_DATA TEXT("/Script/Project_LLL.LLL_MonsterSpawnerDataAsset'/Game/DataAsset/DA_MonsterSpawner.DA_MonsterSpawner'")
#define PATH_MONSTER_SPAWN_DATA TEXT("/Script/Engine.DataTable'/Game/DataTable/MonsterSpawnData.MonsterSpawnData'")
#define PATH_FMOD_PARAMETER_NAME_DATA TEXT("/Script/Engine.DataTable'/Game/DataTable/FModParameterNameData.FModParameterNameData'")

// 맵 및 보상 관련
#define PATH_REWARD_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'")
#define PATH_REWARD_UI_WIDGET TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Reward/WBP_SelectRewordWidget.WBP_SelectRewordWidget_C'")
#define PATH_REWARD_OBJECT_TEST_DATA TEXT("/Script/Project_LLL.LLL_RewardObjectDataAsset'/Game/DataAsset/DA_RewardObject.DA_RewardObject'")

#define PATH_GATE_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/Meshes/Prototyping/SM_GateTest.SM_GateTest'")

#define PATH_PLAYER_GOLD_UI_WIDGET TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Player/WBP_PlayerGoldWidget.WBP_PlayerGoldWidget_C'")
#define PATH_PLAYER_GOLD_COMPONENT_DATA TEXT("/Script/Project_LLL.LLL_GoldComponentDataAsset'/Game/DataAsset/DA_PlayerGoldComponent.DA_PlayerGoldComponent'")

#define PATH_MAP_DATA TEXT("/Script/Project_LLL.LLL_MapDataAsset'/Game/DataAsset/DA_Map.DA_Map'")
#define PATH_ABILITY_DATA_TABLE TEXT("/Script/Engine.DataTable'/Game/DataTable/AbilityDataTable.AbilityDataTable'")
#define PATH_REWARD_DATA_TABLE TEXT("/Script/Engine.DataTable'/Game/DataTable/TestRewardDataTable.TestRewardDataTable'")
#define PATH_UI_PROGRESSBAR_INST TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/Instances/M_UI_CircleProgressBer_Inst.M_UI_CircleProgressBer_Inst'")
