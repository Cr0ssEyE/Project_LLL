
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
#define TAG_GAS_ATTACK_HIT_CHECK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck"))
#define TAG_GAS_ATTACK_HIT_CHECK_COMPLETE FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.CheckComplete"))

// 플레이어 행동, 상태 관련 게임플레이 태그
#define TAG_GAS_PLAYER_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Attack.MeleeAttack"))
#define TAG_GAS_PLAYER_DASH FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Dash"))
#define TAG_GAS_PLAYER_CHASE_THROW FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Throw"))
#define TAG_GAS_PLAYER_CHASE_RUSH FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Rush"))
#define TAG_GAS_PLAYER_CHASE_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Attack"))
#define TAG_GAS_PLAYER_CHASE_RETURN FGameplayTag::RequestGameplayTag(FName("Character.Player.State.ChaseReturned"))
#define TAG_GAS_PLAYER_WAIT_ATTACK_INPUT FGameplayTag::RequestGameplayTag(FName("Character.Player.State.WaitAttackInput"))

// 플레이어 부가 효과 구분용 게임플레이 태그
#define TAG_GAS_PERKS_ATTACK FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Attack")
#define TAG_GAS_PERKS_DASH FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Dash")
#define TAG_GAS_PERKS_SKILL FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Skill")
#define TAG_GAS_PERKS_STAT FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Stat")
#define TAG_GAS_PERKS_CHASE FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Chase")

// 와이어 게임플레이 태그
#define TAG_GAS_CHASE_THROW FGameplayTag::RequestGameplayTag(FName("Object.Chase.Action.Throw"))
#define TAG_GAS_CHASE_RELEASE FGameplayTag::RequestGameplayTag(FName("Object.Chase.Action.Release"))
#define TAG_GAS_CHASE_GRAB FGameplayTag::RequestGameplayTag(FName("Object.Chase.Action.Grab"))
#define TAG_GAS_CHASE_STATE_GRABBED FGameplayTag::RequestGameplayTag(FName("Object.Chase.State.Grabbed"))

// 몬스터 게임플레이 태그
#define TAG_GAS_MONSTER_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Attack"))

// 소드 대시 게임플레이 태그
#define TAG_GAS_SWORD_DASH_DASH FGameplayTag::RequestGameplayTag(FName("Character.Monster.Melee.SwordDash.Action.Dash"))
#define TAG_GAS_SWORD_DASH_CHARGE FGameplayTag::RequestGameplayTag(FName("Character.Monster.Melee.SwordDash.Action.Charge"))

// 시스템 게임플레이 태그
#define TAG_GAS_SYSTEM_DROP_GOLD FGameplayTag::RequestGameplayTag(FName("System.Drop.Gold"))
