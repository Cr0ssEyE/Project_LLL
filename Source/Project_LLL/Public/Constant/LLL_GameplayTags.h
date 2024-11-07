
#pragma once

#include "CoreMinimal.h"

// 캐릭터 이벤트 게임플레이 태그
#define TAG_GAS_COLLIDE_WALL FGameplayTag::RequestGameplayTag(FName("Character.Event.CollideWithWall"))
#define TAG_GAS_DAMAGED FGameplayTag::RequestGameplayTag(FName("Character.Event.Damaged"))

// 캐릭터 상태 관련 게임플레이 태그
#define TAG_GAS_STATUS_BLEEDING FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Bleeding"))
#define TAG_GAS_STATUS_INCREASE_MOVE_SPEED FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.IncreaseMoveSpeed"))
#define TAG_GAS_STATUS_INCREASE_KNOCK_BACK_POWER FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.IncreaseKnockBackPower"))
#define TAG_GAS_STATUS_RESET_ATTACK_SPEED FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.ResetAttackSpeed"))
#define TAG_GAS_STATUS_EVASION FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Evasion"))
#define TAG_GAS_STATUS_RESET_OFFENCE_POWER FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.ResetOffencePower"))
#define TAG_GAS_STATUS_WEAKENING FGameplayTag::RequestGameplayTag(FName("Character.State.AbnormalStatus.Weakening"))

#define TAG_GAS_STATE_COLLIDE_OTHER FGameplayTag::RequestGameplayTag(FName("Character.State.CollideOther"))

// 플레이어 이벤트 게임플레이 태그
#define TAG_GAS_ATTACK_HIT_COUNT FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.ReceiveHitCount"))
#define TAG_GAS_ATTACK_HIT_CHECK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck"))
#define TAG_GAS_SKILL_CHECK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.SkillCheck"))
#define TAG_GAS_ATTACK_HIT_CHECK_MELEE FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.BaseAttack"))
#define TAG_GAS_ATTACK_HIT_CHECK_COMPLETE FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.CheckComplete"))
#define TAG_GAS_ATTACK_HIT_CHECK_SUCCESS FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.AttackHitCheck.Success"))
#define TAG_GAS_FULL_CHARGE_CHECK FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.FullChargeCheck"))
#define TAG_GAS_ANY_ATTACK_SUCCESS FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.AnyAttackSuccess"))

// 플레이어 행동 관련 게임플레이 태그
#define TAG_GAS_PLAYER_DASH_START FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Dash.Start"))
#define TAG_GAS_PLAYER_DASH_END FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Dash.End"))

// 플레이어 상태 관련 게임플레이 태그
#define TAG_GAS_PLAYER_STATE_INPUT_CHECK_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Player.State.InputCheck.BaseAttack"))
#define TAG_GAS_PLAYER_STATE_INPUT_CHECK_DASH FGameplayTag::RequestGameplayTag(FName("Character.Player.State.InputCheck.Dash"))

// 플레이어 공격 효과 관련 게임플레이 태그
#define TAG_GAS_BLEEDING FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.Bleeding"))
#define TAG_GAS_INCREASE_MOVE_SPEED FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.IncreaseMoveSpeed"))
#define TAG_GAS_INCREASE_KNOCK_BACK_POWER FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.IncreaseKnockBackPower"))
#define TAG_GAS_RESET_ATTACK_SPEED FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.ResetAttackSpeed"))
#define TAG_GAS_EVASION FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.Evasion"))
#define TAG_GAS_RESET_OFFENCE_POWER FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.ResetOffencePower"))
#define TAG_GAS_WEAKENING FGameplayTag::RequestGameplayTag(FName("System.AbnormalStatus.Weakening"))

// 플레이어 부가 효과 구분용 게임플레이 태그
#define TAG_GAS_PERKS_ATTACK FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Attack")
#define TAG_GAS_PERKS_DASH FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Dash")
#define TAG_GAS_PERKS_STAT FGameplayTag::RequestGameplayTag(FName("System.Perks.Player.Stat")

// 몬스터 관련 게임플레이 태그
#define TAG_GAS_MONSTER_ATTACK FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Attack"))
#define TAG_GAS_MONSTER_CHARGE FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Charge"))
#define TAG_GAS_MONSTER_FALLABLE FGameplayTag::RequestGameplayTag(FName("Character.Monster.State.Fallable"))
#define TAG_GAS_MONSTER_DASH FGameplayTag::RequestGameplayTag(FName("Character.Monster.Action.Dash"))
#define TAG_GAS_MONSTER_KNOCK_BACKED FGameplayTag::RequestGameplayTag(FName("Character.Monster.Event.KnockBacked"))

// 힘 있는 자 게임플레이 태그
#define TAG_GAS_MAN_OF_STRENGTH_SHOCKWAVE FGameplayTag::RequestGameplayTag(FName("Character.Monster.Boss.ManOfStrength.Action.Shockwave"))
#define TAG_GAS_MAN_OF_STRENGTH_DASH_PUNCH FGameplayTag::RequestGameplayTag(FName("Character.Monster.Boss.ManOfStrength.Action.DashPunch"))
#define TAG_GAS_MAN_OF_STRENGTH_ATTACK_IN_APNEA FGameplayTag::RequestGameplayTag(FName("Character.Monster.Boss.ManOfStrength.Action.AttackInApnea"))
#define TAG_GAS_MAN_OF_STRENGTH_SNAP_OTHER_MONSTER FGameplayTag::RequestGameplayTag(FName("Character.Monster.Boss.ManOfStrength.Action.SnapOtherMonster"))

// 시스템 게임플레이 태그
#define TAG_GAS_SYSTEM_DROP_GOLD FGameplayTag::RequestGameplayTag(FName("System.Drop.Gold"))
#define TAG_GAS_HAVE_CHARGED_FEATHER FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Crow.ChargedFeather"))
#define TAG_GAS_HAVE_RANGED_FEATHER FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Crow.RangedFeather"))
#define TAG_GAS_HAVE_CIRCULAR_FEATHER FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Crow.CircularFeather"))
#define TAG_GAS_HAVE_QUADRUPLE_HIT FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Crow.QuadrupleHit"))
#define TAG_GAS_HAVE_DEFLECT_BY_WALL FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Deer.DeflectByWall"))
#define TAG_GAS_HAVE_KNOCK_BACK_TRANSMISSION FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Deer.KnockBackTransmission"))
#define TAG_GAS_HAVE_INCREASE_KNOCK_BACK_DAMAGE FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Deer.IncreaseKnockBackDamage"))
#define TAG_GAS_HAVE_BLEEDING FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Wolf.Bleeding"))
#define TAG_GAS_HAVE_VAMPIRE FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Wolf.Vampire"))
#define TAG_GAS_HAVE_BLEEDING_TRANSMISSION FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Wolf.BleedingTransmission"))
#define TAG_GAS_HAVE_EXCESSIVE_BLEEDING FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Wolf.ExcessiveBleeding"))
#define TAG_GAS_HAVE_THROW_BLEEDING FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Wolf.ThrowBleeding"))
#define TAG_GAS_HAVE_BLEEDING_EXPLOSION FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Wolf.BleedingExplosion"))
#define TAG_GAS_HAVE_MOVE_FASTER FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Horse.MoveFaster"))
#define TAG_GAS_HAVE_DOUBLE_DASH FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Horse.DoubleDash"))
#define TAG_GAS_HAVE_FASTER_ATTACK FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Horse.FasterAttack"))
#define TAG_GAS_HAVE_EVASION_DASH FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Horse.EvasionDash"))
#define TAG_GAS_HAVE_CHARGE_ATTACK FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.WildBoar.ChargeAttack"))
#define TAG_GAS_HAVE_WAIT_ATTACK FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.WildBoar.WaitAttack"))
#define TAG_GAS_HAVE_INCREASE_KNOCK_BACK_BOTH FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.WildBoar.IncreaseKnockBackBoth"))
#define TAG_GAS_HAVE_DASH_ATTACK FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.WildBoar.DashAttack"))
#define TAG_GAS_HAVE_KNOCK_BACK_WEAKENING FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.WildBoar.KnockBackWeakening"))
#define TAG_GAS_HAVE_ATTACK_WEAKENING FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.WildBoar.AttackWeakening"))
#define TAG_GAS_HAVE_CRISIS_ATTACK FGameplayTag::RequestGameplayTag(FName("System.HaveAbility.Eagle.CrisisAttack"))

// 어빌리티 규칙 관련 태그
#define TAG_GAS_ABILITY_NOT_CANCELABLE FGameplayTag::RequestGameplayTag(FName("Ability.Policy.NotCancelable"))

// 플레이어 보상 어빌리티 관련 게임플레이 태그
#define TAG_GAS_ABILITY_VALUE_1 FGameplayTag::RequestGameplayTag(FName("Ability.Value.1"))
#define TAG_GAS_ABILITY_VALUE_2 FGameplayTag::RequestGameplayTag(FName("Ability.Value.2"))
#define TAG_GAS_ABILITY_HUNDRED_VALUE_1 FGameplayTag::RequestGameplayTag(FName("Ability.HundredValue.1"))
#define TAG_GAS_ABILITY_HUNDRED_VALUE_2 FGameplayTag::RequestGameplayTag(FName("Ability.HundredValue.2"))
#define TAG_GAS_ABILITY_MINUS_VALUE_1 FGameplayTag::RequestGameplayTag(FName("Ability.MinusValue.1"))
#define TAG_GAS_ABILITY_MINUS_VALUE_2 FGameplayTag::RequestGameplayTag(FName("Ability.MinusValue.2"))
#define TAG_GAS_ABILITY_VALUE_OFFENCE_POWER FGameplayTag::RequestGameplayTag(FName("Ability.Value.OffencePower"))
#define TAG_GAS_ABILITY_VALUE_PERIOD FGameplayTag::RequestGameplayTag(FName("Ability.Value.Period"))

// 어빌리티 슬롯 관련 태그
#define TAG_GAS_ABILITY_NESTING_ALLOW FGameplayTag::RequestGameplayTag(FName("Ability.Nesting.Allow"))
#define TAG_GAS_ABILITY_NESTING_DENY FGameplayTag::RequestGameplayTag(FName("Ability.Nesting.Deny"))

// 카메라 관련 태그
#define TAG_SYSTEM_CAMERA_STATE_FOLLOW_CURSOR FGameplayTag::RequestGameplayTag(FName("System.User.Camera.State.FollowCursor"))
#define TAG_SYSTEM_CAMERA_STATE_HOLD_TARGET FGameplayTag::RequestGameplayTag(FName("System.User.Camera.State.HoldToTarget"))

// 큐 관련 태그
#define TAG_GAS_CUE_QUADRUPLE_HIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Object.ThrownObject.ThrownFeather.QuadrupleHit"))
#define TAG_GAS_CUE_MOVE_FASTER FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.Player.MoveFaster"))

// 기타 태그
#define TAG_GAS_STUN_HARD FGameplayTag::RequestGameplayTag(FName("System.StunHard"))
#define TAG_GAS_TEMP_CALLER FGameplayTag::RequestGameplayTag(FName("System.TempCaller"))