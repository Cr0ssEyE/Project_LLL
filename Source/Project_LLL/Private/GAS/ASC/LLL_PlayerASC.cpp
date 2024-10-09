// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC/LLL_PlayerASC.h"

#include "Constant/LLL_GameplayTags.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
ULLL_PlayerASC::ULLL_PlayerASC()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void ULLL_PlayerASC::BeginPlay()
{
	Super::BeginPlay();
	
}

FActiveGameplayEffectHandle ULLL_PlayerASC::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey)
{
	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(Player))
	{
		return Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);
	}
	
	const ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(GameplayEffect.Def);

	if (IsValid(Effect) && (Effect->GetAssetTags().HasTag(TAG_GAS_ABILITY_NESTING_ALLOW) || Effect->GetAssetTags().HasTag(TAG_GAS_ABILITY_NESTING_DENY)))
	{
		const float CoolDown = Effect->GetAbilityData()->AbilityCooldown;
		if (Effect->GetAbilityData()->TagID[1] == '1')
		{
			Player->SetSkillCoolTime(CoolDown);
			Player->ReadyToUseSkill();
			
			EAnimalType AnimalType = Effect->GetAbilityData()->AnimalType;
			if (AnimalType == EAnimalType::Deer || AnimalType == EAnimalType::Wolf)
			{
				Player->SetSkillRotateToMouseCursor(true);
			}
			else
			{
				Player->SetSkillRotateToMouseCursor(false);
			}
		}
		
		const float Value1 = Effect->GetAbilityData()->AbilityValue1 / static_cast<uint32>(Effect->GetAbilityData()->Value1Type);
		const float Value2 = Effect->GetAbilityData()->AbilityValue2 / static_cast<uint32>(Effect->GetAbilityData()->Value2Type);
		if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CHARGED_FEATHER))
		{
			Player->StartChargeFeather(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_DEFLECT_BY_WALL))
		{
			Player->SetDeflectCount(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_KNOCK_BACK_TRANSMISSION))
		{
			Player->SetKnockBackTransmissionOffencePower(Value1);
			Player->SetKnockBackTransmissionKnockBackPower(Value2 * 100.0f);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_QUADRUPLE_HIT))
		{
			Player->SetQuadrupleHitKnockBackPower(Effect->GetAbilityData()->KnockBackPower);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_FASTER_KNOCK_BACK))
		{
			Player->SetFasterKnockBackSpeedRateIncrease(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_INCREASE_KNOCK_BACK_DAMAGE))
		{
			Player->SetIncreaseKnockBackDamageDamageRateIncrease(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_VAMPIRE))
		{
			Player->SetVampireRecoveryRate(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_BLEEDING_TRANSMISSION))
		{
			Player->SetBleedingTransmissionStack(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_EXCESSIVE_BLEEDING))
		{
			Player->SetExcessiveBleedingOffencePowerPlus(Value1);
			Player->SetExcessiveBleedingPeriod(Value2);
			Player->SetExcessiveBleedingWolfEnuriaCheckCount(Effect->GetAbilityData()->RequireSynergy);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_DOUBLE_DASH))
		{
			Player->SetDoubleDashDashRate(Value1);
			Player->SetDoubleDashHorseEnuriaCheckCount(Effect->GetAbilityData()->RequireSynergy);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_EVASION_DASH))
		{
			Player->SetEvasionDashHorseEnuriaCheckCount(Effect->GetAbilityData()->RequireSynergy);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_INCREASE_KNOCK_BACK_BOTH))
		{
			Player->SetIncreaseKnockBackBothKnockBackPowerRate(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_DASH_ATTACK))
		{
			Player->SetDashAttackOffencePowerPlus(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_ATTACK_WEAKENING))
		{
			Player->SetAttackWeakeningOffencePowerRateIncrease(Value2);
			Player->SetAttackWeakeningWildBoarEnuriaCheckCount(Effect->GetAbilityData()->RequireSynergy);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CRISIS_ATTACK))
		{
			Player->SetCrisisAttackMaxOffencePowerRateIncrease(Value1);
		}
	}

	if (GameplayEffect.Def->GetAssetTags().HasTag(TAG_GAS_INCREASE_MOVE_SPEED))
	{
		RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_INCREASE_MOVE_SPEED));
	}
	else if (GameplayEffect.Def->GetAssetTags().HasTag(TAG_GAS_INCREASE_KNOCK_BACK_POWER))
	{
		RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_INCREASE_KNOCK_BACK_POWER));
	}
	else if (GameplayEffect.Def->GetAssetTags().HasTag(TAG_GAS_RESET_ATTACK_SPEED))
	{
		RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_RESET_ATTACK_SPEED));
	}
	else if (GameplayEffect.Def->GetAssetTags().HasTag(TAG_GAS_EVASION))
	{
		RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_EVASION));
	}
	else if (GameplayEffect.Def->GetAssetTags().HasTag(TAG_GAS_RESET_OFFENCE_POWER))
	{
		RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_RESET_OFFENCE_POWER));
	}
	
	return Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);
}
