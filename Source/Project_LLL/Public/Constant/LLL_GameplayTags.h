
#pragma once

#include "CoreMinimal.h"

// 캐릭터 이벤트 게임플레이 태그
#define TAG_GAS_COLLIDE_WALL FGameplayTag::RequestGameplayTag(FName("Character.Event.CollideWithWall"))

// 플레이어 이벤트 게임플레이 태그
#define TAG_GAS_COMBO_ADDITIVE FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.AddCombo"))
#define TAG_GAS_COMBO_SUBTRACTIVE FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.SubtractCombo"))
#define TAG_GAS_COMBO_MANAGEMENT FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.FirstComboStacked"))
#define TAG_GAS_COMBO_CHECK_AMPLIFY FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.ComboCountChanged"))
#define TAG_GAS_SKILL_GAUGE_ADDITIVE FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.AddSkillGauge"))
#define TAG_GAS_SKILL_GAUGE_SUBTRACTIVE FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.SubtractSkillGauge"))

// 플레이어 행동, 상태 관련 게임플레이 태그
#define TAG_GAS_PLAYER_WIRE_THROW FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Wire.Throw"))
#define TAG_GAS_PLAYER_WIRE_RUSH FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Wire.Rush"))
#define TAG_GAS_PLAYER_WIRE_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Wire.Attack"))
#define TAG_GAS_PLAYER_WIRE_RETURN FGameplayTag::RequestGameplayTag(FName("Character.Player.State.WireReturned"))

// 와이어 게임플레이 태그
#define TAG_GAS_WIRE_THROW FGameplayTag::RequestGameplayTag(FName("Object.Wire.Action.Throw"))
#define TAG_GAS_WIRE_RELEASE FGameplayTag::RequestGameplayTag(FName("Object.Wire.Action.Release"))
#define TAG_GAS_WIRE_GRAB FGameplayTag::RequestGameplayTag(FName("Object.Wire.Action.Grab"))
#define TAG_GAS_WIRE_STATE_GRABBED FGameplayTag::RequestGameplayTag(FName("Object.Wire.State.Grabbed"))

// 몬스터 게임플레이 태그
#define TAG_GAS_MONSTER_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Attack"))