#pragma once

#include "CoreMinimal.h"

/* 자잘한 매직넘버들 전처리 목적
 * 주석을 통해 용도 명시 권장
*/

#define BIG_SCALE_SCALAR 10000.f
#define MIDDLE_SCALE_SCALAR 1000.f

// 플레이어 UI 뷰포트 레이어
#define UI_LAYER_BOTTOM 0
#define UI_LAYER_FIRST 1
#define UI_LAYER_SECOND 2
#define UI_LAYER_THIRD 3
#define UI_LAYER_ALWAYS_TOP 99

// 플레이어 UI 텍스트 컨버팅용
#define UI_ABILITY_INFO_ABILITY_VALUE_1 TEXT("AV1")
#define UI_ABILITY_INFO_ABILITY_VALUE_2 TEXT("AV2")
#define UI_ABILITY_INFO_REQUIRE_SYNERGY TEXT("RS")

// 카메라 클리핑 방지용 바운드 스케일
#define ACTOR_DEFAULT_BOUNDS 100.f

// FMOD 마스터 버스
#define FMOD_MASTER_BUS "bus:/"