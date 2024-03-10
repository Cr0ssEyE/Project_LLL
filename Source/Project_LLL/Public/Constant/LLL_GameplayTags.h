
#pragma once

#include "CoreMinimal.h"

// 플레이어 게임플레이 태그
#define TAG_GAS_COMBO_ADDITIVE FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.AddCombo"))
#define TAG_GAS_COMBO_SUBTRACTIVE FGameplayTag::RequestGameplayTag(FName("Character.Player.Event.SubtractCombo"))
#define TAG_GAS_COMBO_MANAGEMENT FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Event.ComboValueChanged"))
#define TAG_GAS_PLAYER_WIRE_RUSH FGameplayTag::RequestGameplayTag(FName("Character.Player.Action.Wire.Rush"))
#define TAG_GAS_PLAYER_WIRE_RETURN FGameplayTag::RequestGameplayTag(FName("Character.Player.State.WireReturned"))

// 와이어 게임플레이 태그
#define TAG_GAS_WIRE_THROW FGameplayTag::RequestGameplayTag(FName("Object.Wire.Action.Throw"))
#define TAG_GAS_WIRE_RELEASE FGameplayTag::RequestGameplayTag(FName("Object.Wire.Action.Release"))
#define TAG_GAS_WIRE_GRAB FGameplayTag::RequestGameplayTag(FName("Object.Wire.Action.Grab"))
#define TAG_GAS_WIRE_STATE_GRABBED FGameplayTag::RequestGameplayTag(FName("Object.Wire.State.Grabbed"))