#pragma once

#include "CoreMinimal.h"

// UI 머티리얼 파라미터 이름
#define UI_PROGRESS_TOP TEXT("TopProgress")
#define UI_PROGRESS_BOTTOM TEXT("BottomProgress")
#define UI_RARITY_COLOR TEXT("RarityColor")
#define UI_ERURIA_ICON TEXT("EruriaIcon")

#define PP_PLAYER_LOW_HP_RADIUS TEXT("PP_PlayerLowHP_Radius")

// 범용 파라미터 이름
#define MAT_PARAM_TEXTURE TEXT("Texture")
#define MAT_PARAM_COLOR TEXT("Color")
#define MAT_PARAM_OPACITY TEXT("Opacity")

// 나이아가라 파라미터 이름
#define NS_MARK_COUNT TEXT("MarkCount")
#define NS_WAVE_RADIUS TEXT("WaveRadius")
#define NS_LIFE_TIME TEXT("LifeTime")

// 플레이어 관련
#define PLAYER_CHARACTER_DISSOLVE TEXT("PlayerCharacterDissolve")

/* 커스텀 스텐실 값
 * 툰 셰이딩이 적용 되는 대상: 2의 배수
 * 불릿타임에 영향을 받지 않는 대상: 4 이상의 값
*/

#define STENCIL_VALUE_TOON_BASE 2
#define STENCIL_VALUE_PLAYER 4