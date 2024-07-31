// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// 에디터 편의성
#define PATH_DUMMY_TABLE TEXT("/Game/DataTable/AttributeInitializer/CT_TableUpdateOnlyData.CT_TableUpdateOnlyData")

// 어빌리티 관련
#define PATH_ABILITY_DATA_TABLE TEXT("/Script/Engine.DataTable'/Game/DataTable/DT_AbilityData.DT_AbilityData'")
#define PATH_PLAYER_EFFECTS TEXT("PlayerEffects")
#define PATH_MONSTER_EFFECTS TEXT("MonsterEffects")
#define PATH_OBJECT_EFFECTS TEXT("ObjectEffects")
#define PATH_SHARE_EFFECTS TEXT("ShareEffects")

// 플레이어 관련
#define PATH_PLAYER_DATA TEXT("/Script/Project_LLL.LLL_PlayerBaseDataAsset'/Game/DataAsset/DA_Player.DA_Player'")
#define PATH_CAMERA_DATA TEXT("/Script/Project_LLL.LLL_CameraDataAsset'/Game/DataAsset/DA_Camera.DA_Camera'")
#define PATH_PLAYER_GOLD_COMPONENT_DATA TEXT("/Script/Project_LLL.LLL_GoldComponentDataAsset'/Game/DataAsset/DA_PlayerGoldComponent.DA_PlayerGoldComponent'")
#define PATH_PLAYER_LOWHP_MATERIAL_INST TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/PostProcess/PPMI_LowHP.PPMI_LowHP'")

// 플레이어 어빌리티 오브젝트 관련
#define PATH_FEATHER_STORM_DATA TEXT("/Script/Project_LLL.LLL_FeatherStormDataAsset'/Game/DataAsset/DA_FeatherStorm.DA_FeatherStorm'")
#define PATH_THREATENING_DEATH_DATA TEXT("/Script/Project_LLL.LLL_ThreateningDeathDataAsset'/Game/DataAsset/DA_ThreateningDeath.DA_ThreateningDeath'")

// 플레이어 투사체 관련
#define PATH_PLAYER_CHASE_HAND_DATA TEXT("/Script/Project_LLL.LLL_PlayerChaseHandDataAsset'/Game/DataAsset/DA_PlayerChaseHand.DA_PlayerChaseHand'")
#define PATH_THROWN_FEATHER_DATA TEXT("/Script/Project_LLL.LLL_ThrownFeatherDataAsset'/Game/DataAsset/DA_ThrownFeather.DA_ThrownFeather'")

// 몬스터 캐릭터 관련
#define PATH_SWORD_DASH_DATA TEXT("/Script/Project_LLL.LLL_SwordDashDataAsset'/Game/DataAsset/DA_SwordDash.DA_SwordDash'")
#define PATH_STAFF_BASIC_DATA TEXT("/Script/Project_LLL.LLL_StaffBasicDataAsset'/Game/DataAsset/DA_StaffBasic.DA_StaffBasic'")
#define PATH_CLAW_BASIC_DATA TEXT("/Script/Project_LLL.LLL_ClawBasicDataAsset'/Game/DataAsset/DA_ClawBasic.DA_ClawBasic'")

// 몬스터 투사체 관련
#define PATH_THROWN_MAGIC_DATA TEXT("/Script/Project_LLL.LLL_ThrownMagicDataAsset'/Game/DataAsset/DA_ThrownMagic.DA_ThrownMagic'")

// 오브젝트 관련
#define PATH_INTERACTIVE_OBJECT_DATA TEXT("/Script/Project_LLL.LLL_InteractiveObjectData'/Game/DataAsset/DA_InteractiveObject.DA_InteractiveObject'")
#define PATH_BREAKABLE_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_Statue.SM_Statue'")
#define PATH_BREAKABLE_OBJECT_TEST_EFFECT TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/DropGold/BPGE_DropGold.BPGE_DropGold_C'")

// 시스템 관련
#define PATH_MONSTER_SPAWNER_DATA TEXT("/Script/Project_LLL.LLL_MonsterSpawnerDataAsset'/Game/DataAsset/DA_MonsterSpawner.DA_MonsterSpawner'")
#define PATH_MONSTER_SPAWN_DATA TEXT("/Script/Engine.DataTable'/Game/DataTable/MonsterSpawnData.MonsterSpawnData'")
#define PATH_FMOD_PARAMETER_NAME_DATA TEXT("/Script/Engine.DataTable'/Game/DataTable/FModParameterNameData.FModParameterNameData'")
#define PATH_STRING_DATA TEXT("/Script/Engine.DataTable'/Game/DataTable/MiscellaneousDatas/DT_StringData.DT_StringData'")
#define PATH_GLOBAL_NIAGARA_EFFECT_DATA TEXT("/Script/Project_LLL.LLL_GlobalNiagaraDataAsset'/Game/DataAsset/Global/DA_GlobalNiagaraEffectData.DA_GlobalNiagaraEffectData'")
#define PATH_GLOBAL_PARAMETERS_DATA TEXT("/Script/Project_LLL.LLL_GlobalParameterDataAsset'/Game/DataAsset/Global/DA_GlobalSystemData.DA_GlobalSystemData'")
#define PATH_GLOBAL_SEQUENCE_DATA TEXT("/Script/Project_LLL.LLL_GlobalLevelSequenceDataAsset'/Game/DataAsset/Global/DA_GlobalSequenceData.DA_GlobalSequenceData'")

// 맵 및 보상 시스템 관련
#define PATH_MAP_DATA TEXT("/Script/Project_LLL.LLL_MapDataAsset'/Game/DataAsset/DA_Map.DA_Map'")
#define PATH_GATE_DATA TEXT("/Script/Project_LLL.LLL_GateDataAsset'/Game/DataAsset/DA_Gate.DA_Gate'")
#define PATH_REWARD_DATA_TABLE TEXT("/Script/Engine.DataTable'/Game/DataTable/RewardDataTable.RewardDataTable'")
#define PATH_REWARD_OBJECT_TEST_MESH TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'")
#define PATH_REWARD_OBJECT_TEST_DATA TEXT("/Script/Project_LLL.LLL_RewardObjectDataAsset'/Game/DataAsset/DA_RewardObject.DA_RewardObject'")
#define PATH_TUTORIAL_MAP_DATA TEXT("/Script/Project_LLL.LLL_TutorialMapDataAsset'/Game/DataAsset/DA_TutorialMap.DA_TutorialMap'")
#define PATH_SKILL_GAUGE_EFFECT TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/Character/Player/Status/BPGE_Player_FullSkillGauge.BPGE_Player_FullSkillGauge_C'")
#define PATH_MAP_SOUND_SUBSYSTEM_DATA TEXT("/Script/Project_LLL.LLL_MapSoundSubsystemDataAsset'/Game/DataAsset/DA_MapSoundSubsystem.DA_MapSoundSubsystem'")

// UI 관련
#define PATH_REWARD_UI_WIDGET TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Reward/WBP_SelectRewordWidget.WBP_SelectRewordWidget_C'")
#define PATH_PLAYER_GOLD_UI_WIDGET TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Player/WBP_PlayerGoldWidget.WBP_PlayerGoldWidget_C'")
#define PATH_UI_PROGRESSBAR_MATERIAL TEXT("/Script/Engine.Material'/Game/Materials/MasterMaterials/M_UI_CircleProgressBer.M_UI_CircleProgressBer'")

// 머티리얼 파라미터 관련
#define PATH_MONSTER_MPC TEXT("/Script/Engine.MaterialParameterCollection'/Game/Materials/MaterialParameters/MPC_Monster.MPC_Monster'")
#define PATH_OBJECT_MPC TEXT("/Script/Engine.MaterialParameterCollection'/Game/Materials/MaterialParameters/MPC_Object.MPC_Object'")
#define PATH_PLAYER_MPC TEXT("/Script/Engine.MaterialParameterCollection'/Game/Materials/MaterialParameters/MPC_Player.MPC_Player'")
#define PATH_POSTPROCESS_MPC TEXT("/Script/Engine.MaterialParameterCollection'/Game/Materials/MaterialParameters/MPC_PostProcess.MPC_PostProcess'")
