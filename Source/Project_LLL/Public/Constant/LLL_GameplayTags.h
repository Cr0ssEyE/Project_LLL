﻿
#pragma once

#include "CoreMinimal.h"

// 캐릭터 이벤트 게임플레이 태그
#define TAG_GAS_COLLIDE_WALL FGameplayTag::RequestGameplayTag(FName("Character.Event.CollideWithWall"))
#define TAG_GAS_DAMAGED FGameplayTag::RequestGameplayTag(FName("Character.Event.Damaged"))

// 캐릭터 상태 관련 게임플레이 태그
#define TAG_GAS_STATUS_BLEEDING FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Bleeding"))
#define TAG_GAS_STATUS_BLEEDING_BASE_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Bleeding.FromBaseAttack"))
#define TAG_GAS_STATUS_BLEEDING_CHASE_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Bleeding.FromChaseAttack"))
#define TAG_GAS_STATUS_BLEEDING_DASH_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Bleeding.FromDash"))
#define TAG_GAS_STATUS_MARKED FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Marked"))
#define TAG_GAS_STATUS_TARGETED FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Targeted"))
#define TAG_GAS_STATE_KNOCKBACKED FGameplayTag::RequestGameplayTag(FName("Character.State.IsKnockBacked"))
#define TAG_GAS_STATE_COLLIDE_OTHER FGameplayTag::RequestGameplayTag(FName("Character.State.CollideOther"))

// 플레이어 이벤트 게임플레이 태그
#define TAG_GAS_COMBO_MANAGEMENT FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.FirstComboStacked"))
#define TAG_GAS_COMBO_CHECK_AMPLIFY FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.ComboCountChanged"))
#define TAG_GAS_ATTACK_HIT_COUNT FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.ReceiveHitCount"))
#define TAG_GAS_ATTACK_HIT_CHECK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck"))
#define TAG_GAS_ATTACK_HIT_CHECK_MELEE FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.BaseAttack"))
#define TAG_GAS_ATTACK_HIT_CHECK_CHASE FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.ChaseAttack"))
#define TAG_GAS_ATTACK_HIT_CHECK_COMPLETE FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.CheckComplete"))
#define TAG_GAS_ATTACK_HIT_CHECK_SUCCESS FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.Success"))

// 플레이어 행동 관련 게임플레이 태그
#define TAG_GAS_PLAYER_DASH_START FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Dash.Start"))
#define TAG_GAS_PLAYER_DASH_END FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Dash.End"))
#define TAG_GAS_PLAYER_BULLET_TIME_START FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Skill.ActivateBulletTime"))
#define TAG_GAS_PLAYER_BULLET_TIME_END FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Skill.DeactivateBulletTime"))
#define TAG_GAS_PLAYER_CHASE_THROW FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Throw"))
#define TAG_GAS_PLAYER_CHASE_RUSH FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Rush"))
#define TAG_GAS_PLAYER_CHASE_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.Attack"))
#define TAG_GAS_PLAYER_CHASER_THROW FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.ChaserThrow"))
#define TAG_GAS_PLAYER_THROW_TRIGGERED FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Chase.ChaserThrow.Trigger"))

// 플레이어 상태 관련 게임플레이 태그
#define TAG_GAS_PLAYER_STATE_CHASE_COOLDOWN FGameplayTag::RequestGameplayTag(FName("Character.Player.State.ChaserCoolDown"))
#define TAG_GAS_PLAYER_STATE_CHASE_THREW FGameplayTag::RequestGameplayTag(FName("Character.Player.State.ChaserThrew"))
#define TAG_GAS_PLAYER_STATE_CHASE_RETURN FGameplayTag::RequestGameplayTag(FName("Character.Player.State.ChaserReturned"))
#define TAG_GAS_PLAYER_STATE_CHASE_PROGRESS FGameplayTag::RequestGameplayTag(FName("Character.Player.State.ChaseAttackOnGoing"))
#define TAG_GAS_PLAYER_STATE_INPUT_CHECK_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Player.State.InputCheck.BaseAttack"))
#define TAG_GAS_PLAYER_STATE_INPUT_CHECK_DASH FGameplayTag::RequestGameplayTag(FName("Character.Player.State.InputCheck.Dash"))

// 플레이어 공격 효과 관련 게임플레이 태그
#define TAG_GAS_ABNORMAL_STATUS FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus"))
#define TAG_GAS_BLEEDING FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.Bleeding"))
#define TAG_GAS_MARK FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.Mark"))
#define TAG_GAS_TARGETING FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.Targeting"))
#define TAG_GAS_MARK_STACK FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.Mark.Stack"))

// 플레이어 부가 효과 구분용 게임플레이 태그
#define TAG_GAS_PERKS_ATTACK FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Attack")
#define TAG_GAS_PERKS_DASH FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Dash")
#define TAG_GAS_PERKS_SKILL FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Skill")
#define TAG_GAS_PERKS_STAT FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Stat")
#define TAG_GAS_PERKS_CHASE FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Chase")

// 체이서 게임플레이 태그
#define TAG_GAS_CHASER_THROW FGameplayTag::RequestGameplayTag(FName("Object.Chaser.Action.Throw"))
#define TAG_GAS_CHASER_RELEASE FGameplayTag::RequestGameplayTag(FName("Object.Chaser.Action.Release"))
#define TAG_GAS_CHASER_GRAB FGameplayTag::RequestGameplayTag(FName("Object.Chaser.Action.Grab"))
#define TAG_GAS_CHASER_STATE_THROWING FGameplayTag::RequestGameplayTag(FName("Object.Chaser.State.Throwing"))
#define TAG_GAS_CHASER_STATE_GRABBED FGameplayTag::RequestGameplayTag(FName("Object.Chaser.State.Grabbed"))

// 몬스터 관련 게임플레이 태그
#define TAG_GAS_MONSTER_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Attack"))
#define TAG_GAS_MONSTER_CHARGE FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Charge"))
#define TAG_GAS_MONSTER_FALLABLE FGameplayTag::RequestGameplayTag(FName("Character.Monster.State.Fallable"))

// 소드 대시 게임플레이 태그
#define TAG_GAS_SWORD_DASH_DASH FGameplayTag::RequestGameplayTag(FName("Character.Monster.Melee.SwordDash.Action.Dash"))

// 시스템 게임플레이 태그
#define TAG_GAS_SYSTEM_DROP_GOLD FGameplayTag::RequestGameplayTag(FName("System.Drop.Gold"))
#define TAG_GAS_HAVE_FEATHER_AMPLIFICATION FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Crow.FeatherAmplification"))

// 어빌리티 규칙 관련 태그
#define TAG_GAS_ABILITY_NOT_CANCELABLE FGameplayTag::RequestGameplayTag(FName("Ability.Policy.NotCancelable"))

// 플레이어 보상 어빌리티 관련 게임플레이 태그
#define TAG_GAS_ABILITY_CHANGEABLE_VALUE FGameplayTag::RequestGameplayTag(FName("Ability.Value.ChangeableValue"))
#define TAG_GAS_ABILITY_UNCHANGEABLE_VALUE FGameplayTag::RequestGameplayTag(FName("Ability.Value.UnChangeableValue"))

// 어빌리티 슬롯 관련 태그
#define TAG_GAS_ABILITY_PART FGameplayTag::RequestGameplayTag(FName("AbilityPart"))
#define TAG_GAS_ABILITY_PART_COMMON FGameplayTag::RequestGameplayTag(FName("AbilityPart.Common"))
#define TAG_GAS_ABILITY_PART_ATTACK FGameplayTag::RequestGameplayTag(FName("AbilityPart.Attack"))
#define TAG_GAS_ABILITY_PART_CHASE FGameplayTag::RequestGameplayTag(FName("AbilityPart.Chase"))
#define TAG_GAS_ABILITY_PART_COMBO_SKILL FGameplayTag::RequestGameplayTag(FName("AbilityPart.ComboSkill"))
#define TAG_GAS_ABILITY_PART_DASH FGameplayTag::RequestGameplayTag(FName("AbilityPart.Dash"))
#define TAG_GAS_ABILITY_PART_GRANT FGameplayTag::RequestGameplayTag(FName("AbilityPart.Event.Granted"))
#define TAG_GAS_ABILITY_PART_REMOVE FGameplayTag::RequestGameplayTag(FName("AbilityPart.Event.Removed"))

// 카메라 관련 태그
#define TAG_SYSTEM_CAMERA_STATE_FOLLOW_CURSOR FGameplayTag::RequestGameplayTag(FName("System.User.Camera.State.FollowCursor"))
#define TAG_SYSTEM_CAMERA_STATE_HOLD_TARGET FGameplayTag::RequestGameplayTag(FName("System.User.Camera.State.HoldToTarget"))