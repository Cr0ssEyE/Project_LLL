
#pragma once

#include "CoreMinimal.h"

// 캐릭터 이벤트 게임플레이 태그
#define TAG_GAS_COLLIDE_WALL FGameplayTag::RequestGameplayTag(FName("Character.Event.CollideWithWall"))
#define TAG_GAS_DAMAGED FGameplayTag::RequestGameplayTag(FName("Character.Event.Damaged"))

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
#define TAG_GAS_PLAYER_DASH FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Dash"))
#define TAG_GAS_PLAYER_CHASE_THROW FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Throw"))
#define TAG_GAS_PLAYER_CHASE_RUSH FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Rush"))
#define TAG_GAS_PLAYER_CHASE_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Attack"))
#define TAG_GAS_PLAYER_STATE_CHASE_THREW FGameplayTag::RequestGameplayTag(FName("Character.Player.State.ChaseThrew"))
#define TAG_GAS_PLAYER_STATE_CHASE_RETURNED FGameplayTag::RequestGameplayTag(FName("Character.Player.State.ChaseReturned"))
#define TAG_GAS_PLAYER_STATE_WAIT_ATTACK_INPUT FGameplayTag::RequestGameplayTag(FName("Character.Player.State.WaitAttackInput"))

// 플레이어 공격 효과 관련 게임플레이 태그 (SetByCaller)
#define TAG_GAS_BLEEDING_DAMAGE FGameplayTag::RequestGameplayTag(FName("System.AbnromalStatus.Bleeding.Damage"))
#define TAG_GAS_MARK_STACK FGameplayTag::RequestGameplayTag(FName("System.AbnromalStatus.Mark.Stack"))

// 플레이어 보상 어빌리티 관련 게임플레이 태그 (SetByCaller)
#define TAG_GAS_EFFECT_DURATION FGameplayTag::RequestGameplayTag(FName("System.Ability.Duration"))

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

// 몬스터 관련 게임플레이 태그
#define TAG_GAS_MONSTER_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Attack"))
#define TAG_GAS_MONSTER_CHARGE FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Charge"))
#define TAG_GAS_MONSTER_FALLABLE FGameplayTag::RequestGameplayTag(FName("Character.Monster.State.Fallable"))

// 소드 대시 게임플레이 태그
#define TAG_GAS_SWORD_DASH_DASH FGameplayTag::RequestGameplayTag(FName("Character.Monster.Melee.SwordDash.Action.Dash"))

// 시스템 게임플레이 태그
#define TAG_GAS_SYSTEM_DROP_GOLD FGameplayTag::RequestGameplayTag(FName("System.Drop.Gold"))

// 어빌리티 규칙 관련 태그
#define TAG_GAS_ABILITY_NOT_CANCELABLE FGameplayTag::RequestGameplayTag(FName("Ability.Policy.NotCancelable"))

// 어빌리티 슬롯 관련 태그
#define TAG_GAS_ABILITY_PART FGameplayTag::RequestGameplayTag(FName("AbilityPart"))
#define TAG_GAS_ABILITY_PART_COMMON FGameplayTag::RequestGameplayTag(FName("AbilityPart.Common"))