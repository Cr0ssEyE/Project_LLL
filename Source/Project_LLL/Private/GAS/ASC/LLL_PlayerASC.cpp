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

	if (IsValid(Effect) && (Effect->GetAssetTags().HasTag(TAG_GAS_ABILITY_NESTING_DENY) || Effect->GetAssetTags().HasTag(TAG_GAS_ABILITY_NESTING_DENY)))
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
			Player->SetFasterKnockBackSpeedRate(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_INCREASE_KNOCK_BACK_DAMAGE_BY_ENURIA_COUNT))
		{
			Player->SetIncreaseKnockBackDamageByEnuriaCountDamageRate(Value1);
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
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_BLEEDING_EXPLOSION))
		{
			Player->SetBleedingExplosionOffencePower(Value1);
		}
		else if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_DOUBLE_DASH))
		{
			Player->SetDoubleDashDashRate(Value1);
			Player->SetDoubleDashHorseEnuriaCheckCount(Effect->GetAbilityData()->RequireSynergy);
		}
	}

	if (GameplayEffect.Def->GetAssetTags().HasTag(TAG_GAS_MOVE_FASTER))
	{
		RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_MOVE_FASTER));
	}
	else if (GameplayEffect.Def->GetAssetTags().HasTag(TAG_GAS_MORE_ATTACK_KNOCK_BACK))
	{
		RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_MORE_ATTACK_KNOCK_BACK));
	}
	
	return Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);
}
