// Fill out your copyright notice in the Description page of Project Settings.

	#pragma once

#include "CoreMinimal.h"

// 에디터 편의성
#define PATH_DUMMY_TABLE TEXT("/Game/DataTable/AttributeInitializer/CT_TableUpdateOnlyData.CT_TableUpdateOnlyData")

// 어빌리티 관련
#define PATH_ABILITY_DATA_TABLE TEXT("/Script/Engine.DataTable'/Game/DataTable/AbilityDataTable.AbilityDataTable'")
#define PATH_PLAYER_EFFECTS TEXT("PlayerEffects")
#define PATH_MONSTER_EFFECTS TEXT("MonsterEffects")
#define PATH_OBJECT_EFFECTS TEXT("ObjectEffects")
#define PATH_SHARE_EFFECTS TEXT("ShareEffects")

// 플레이어 관련
#define PATH_PLAYER_DATA TEXT("/Script/Project_LLL.LLL_PlayerBaseDataAsset'/Game/DataAsset/DA_Player.DA_Player'")
#define PATH_CAMERA_DATA TEXT("/Script/Project_LLL.LLL_CameraDataAsset'/Game/DataAsset/DA_Camera.DA_Camera'")
#define PATH_PLAYER_GOLD_COMPONENT_DATA TEXT("/Script/Project_LLL.LLL_GoldComponentDataAsset'/Game/DataAsset/DA_PlayerGoldComponent.DA_PlayerGoldComponent'")

// 플레이어 스킬 관련
#define PATH_PLAYER_FEATHER_STORM_DATA TEXT("/Script/Project_LLL.LLL_PlayerFeatherStormDataAsset'/Game/DataAsset/DA_PlayerFeatherStorm.DA_PlayerFeatherStorm'")
#define PATH_PLAYER_PHYSICAL_PROOF_DATA TEXT("/Script/Project_LLL.LLL_PlayerPhysicalProofDataAsset'/Game/DataAsset/DA_PlayerPhysicalProof.DA_PlayerPhysicalProof'")

// 플레이어 투사체 관련
#define PATH_PLAYER_CHASE_HAND_DATA TEXT("/Script/Project_LLL.LLL_PlayerChaseHandDataAsset'/Game/DataAsset/DA_PlayerChaseHand.DA_PlayerChaseHand'")
#define PATH_PLAYER_THROWN_FEATHER_DATA TEXT("/Script/Project_LLL.LLL_PlayerThrownFeatherDataAsset'/Game/DataAsset/DA_PlayerThrownFeather.DA_PlayerThrownFeather'")

// 몬스터 캐릭터 관련
#define PATH_SWORD_DASH_DATA TEXT("/Script/Project_LLL.LLL_SwordDashDataAsset'/Game/DataAsset/DA_SwordDash.DA_SwordDash'")
#define PATH_STAFF_BASIC_DATA TEXT("/Script/Project_LLL.LLL_StaffBasicDataAsset'/Game/DataAsset/DA_StaffBasic.DA_StaffBasic'")
#define PATH_CLAW_BASIC_DATA TEXT("/Script/Project_LLL.LLL_ClawBasicDataAsset'/Game/DataAsset/DA_ClawBasic.DA_ClawBasic'")

// 몬스터 투사체 관련
#define PATH_ENERGY_BLASTER_ENERGY_DATA TEXT("/Script/Project_LLL.LLL_EnergyBlasterEnergyDataAsset'/Game/DataAsset/DA_EnergyBlasterEnergy.DA_EnergyBlasterEnergy'")
#define PATH_STAFF_BASIC_MAGIC_DATA TEXT("/Script/Project_LLL.LLL_StaffBasicMagicDataAsset'/Game/DataAsset/DA_StaffBasicMagic.DA_StaffBasicMagic'")

// 오브젝트 관련
#define PATH_INTERACTIVE_OBJECT_DATA TEXT("/Script/Project_LLL.LLL_InteractiveObjectData'/Game/DataAsset/DA_InteractiveObject.DA_InteractiveObject'")
#define PATH_BREAKABLE_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'")
#define PATH_BREAKABLE_OBJECT_TEST_EFFECT TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/DropGold/BPGE_DropGold.BPGE_DropGold_C'")

// 시스템 관련
#define PATH_MONSTER_SPAWNER_DATA TEXT("/Script/Project_LLL.LLL_MonsterSpawnerDataAsset'/Game/DataAsset/DA_MonsterSpawner.DA_MonsterSpawner'")
#define PATH_MONSTER_SPAWN_DATA TEXT("/Script/Engine.DataTable'/Game/DataTable/MonsterSpawnData.MonsterSpawnData'")
#define PATH_FMOD_PARAMETER_NAME_DATA TEXT("/Script/Engine.DataTable'/Game/DataTable/FModParameterNameData.FModParameterNameData'")

// 맵 및 보상 시스템 관련
#define PATH_MAP_DATA TEXT("/Script/Project_LLL.LLL_MapDataAsset'/Game/DataAsset/DA_Map.DA_Map'")
#define PATH_GATE_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/Meshes/Prototyping/SM_GateTest.SM_GateTest'")
#define PATH_REWARD_DATA_TABLE TEXT("/Script/Engine.DataTable'/Game/DataTable/RewardDataTable.RewardDataTable'")
#define PATH_REWARD_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'")
#define PATH_REWARD_OBJECT_TEST_DATA TEXT("/Script/Project_LLL.LLL_RewardObjectDataAsset'/Game/DataAsset/DA_RewardObject.DA_RewardObject'")

// UI 관련
#define PATH_REWARD_UI_WIDGET TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Reward/WBP_SelectRewordWidget.WBP_SelectRewordWidget_C'")
#define PATH_PLAYER_GOLD_UI_WIDGET TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Player/WBP_PlayerGoldWidget.WBP_PlayerGoldWidget_C'")
#define PATH_UI_PROGRESSBAR_MATERIAL TEXT("/Script/Engine.Material'/Game/Materials/MasterMaterials/M_UI_CircleProgressBer.M_UI_CircleProgressBer'")
