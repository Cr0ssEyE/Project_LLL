// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_PlayerCharacterAttributeSet::ULLL_PlayerCharacterAttributeSet() :
LowHealthPercentage(0.3f),
TargetingCorrectionRadius(100.f),
AllOffencePowerRate(1.0f),
AllOffencePowerPlus(0.0f),
KnockBackPowerRate(1.0f),
KnockBackPowerPlus(0.0f),
KnockBackOffencePowerRate(1.0f),
KnockBackOffencePowerPlus(0.0f),
FeatherOffencePowerRate(1.0f),
FeatherOffencePowerPlus(0.0f),
MoveSpeedPlus(0.0f),
BaseAttackKnockBackPowerPlus(0.0f),
DashDistancePlus(0.0f),
FasterAttackAttackSpeedRate(1.0f),
BleedingExplosionOffencePower(0.0f),
CriticalChancePlus(0.0f),
FeatherManaRecoveryValue(0.0f)

//임시
,MaxMana(100),
CurrentMana(0),
ChargeAttack1ManaCost(1),
ChargeAttack2ManaCost(1),
OffencePowerRate1(2),
OffencePowerRate2(2),
OffencePowerRate3(2),
OffencePowerRate4(2),
AttackManaRecoveryValue(5.0f)
{
	
}

FPlayerCharacterStatusData ULLL_PlayerCharacterAttributeSet::MakeCharacterStatusData() const
{
	FPlayerCharacterStatusData CharacterStatusData;
	CharacterStatusData.MaxHealth = MaxHealth.GetBaseValue();
	CharacterStatusData.CurrentHealth = CurrentHealth.GetCurrentValue();
	CharacterStatusData.OffencePower = OffencePower.GetBaseValue();
	CharacterStatusData.MoveSpeed = MoveSpeed.GetBaseValue();
	CharacterStatusData.AttackSpeed = AttackSpeed.GetBaseValue();
	CharacterStatusData.CriticalChance = CriticalChance.GetBaseValue();
	CharacterStatusData.CriticalAmplify = CriticalAmplify.GetBaseValue();
	CharacterStatusData.MaxDashCount = MaxDashCount.GetBaseValue();
	CharacterStatusData.DashDistance = DashDistance.GetBaseValue();
	CharacterStatusData.KnockBackPower1 = KnockBackPower1.GetBaseValue();
	CharacterStatusData.KnockBackPower2 = KnockBackPower2.GetBaseValue();
	CharacterStatusData.KnockBackConstant = KnockBackConstant.GetBaseValue();
	
	return CharacterStatusData;
}

void ULLL_PlayerCharacterAttributeSet::InitializeSavedStatusData(const FPlayerCharacterStatusData* CharacterStatusData)
{
	MaxHealth.SetCurrentValue(CharacterStatusData->MaxHealth);
	CurrentHealth.SetCurrentValue(CharacterStatusData->CurrentHealth); // (FMath::Clamp(CharacterStatusData->CurrentHealth, 0, CharacterStatusData->MaxHealth));
	OffencePower.SetBaseValue(CharacterStatusData->OffencePower);
	MoveSpeed.SetBaseValue(CharacterStatusData->MoveSpeed);
	AttackSpeed.SetBaseValue(CharacterStatusData->AttackSpeed);
	CriticalChance.SetBaseValue(CharacterStatusData->CriticalChance);
	CriticalAmplify.SetBaseValue(CharacterStatusData->CriticalAmplify);
	MaxDashCount.SetBaseValue(CharacterStatusData->MaxDashCount);
	DashDistance.SetBaseValue(CharacterStatusData->DashDistance);
	KnockBackPower1.SetBaseValue(CharacterStatusData->KnockBackPower1);
	KnockBackPower2.SetBaseValue(CharacterStatusData->KnockBackPower2);
	KnockBackConstant.SetBaseValue(CharacterStatusData->KnockBackConstant);

	const ALLL_BaseCharacter* OwnerCharacter = CastChecked<ALLL_BaseCharacter>(GetOwningActor());
	OwnerCharacter->UpdateWidgetDelegate.Broadcast();
}

void ULLL_PlayerCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetEvasionRateAttribute())
	{
		RecalculateEvasion(NewValue);
	}

	if (Attribute == GetEvasionFlagAttribute())
	{
		ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetOwningActor());
		if (const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent())
		{
			if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_EVASION_DASH))
			{
				const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
				if (NewValue)
				{
					Player->ParticleDurationActivate(PlayerDataAsset->EvasionDashParticle, Player->GetEvasionDashTimer());
				}
				else
				{
					Player->ParticleDeactivate(PlayerDataAsset->EvasionDashParticle);
				}
			}
		}
	}
}

void ULLL_PlayerCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetOwningActor());
		if (Player->CheckCharacterIsDead())
		{
			return;
		}
		
		ALLL_BaseCharacter* Attacker = CastChecked<ALLL_BaseCharacter>(Data.EffectSpec.GetEffectContext().Get()->GetInstigator());
		const bool DOT = Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckPlayerIsInvincible())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("플레이어 무적 상태")));
				Player->Damaged(Attacker, DOT);
				Player->TakeDamageDelegate.Broadcast(DOT);

				FGameplayEventData PayloadData;
				PayloadData.Instigator = Attacker;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), TAG_GAS_DAMAGED, PayloadData);
				return;
			}
		}
#endif
		
		bool bIsEvasion = static_cast<bool>(GetEvasionFlag());
		RecalculateEvasion(GetEvasionRate());
		
		if (Player->GetCapsuleComponent()->GetCollisionProfileName() == CP_PLAYER_EVADE)
		{
			bIsEvasion = true;
		}
		
		if (!bIsEvasion)
		{
			SetReceiveDamage(GetReceiveDamage() * GetReceiveDamageRate());

			// 과충전 이누리아
			const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
			if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_CHARGE_ATTACK) && Player->CheckChargeTriggered())
			{
				SetReceiveDamage(GetReceiveDamage() * (1 - Player->GetChargeAttackReceiveDamageRateDecrease()));
			}
			
			SetReceiveDamage(FMath::Floor(GetReceiveDamage()));
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetReceiveDamage(), 0.f, GetMaxHealth()));
			if (GetCurrentHealth() == 0 && !Player->CheckCharacterIsDead())
			{
				Player->Dead();
			}
			else
			{
				Player->Damaged(Attacker, DOT);
			}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (DebugGameInstance->CheckPlayerHitDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("플레이어 데미지 입음. : %f"), Data.EvaluatedData.Magnitude));
				}
			}
#endif
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("%.2f 확률로 회피 발동"), GetEvasionRate() * 100.0f)
			return;
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentManaAttribute())
	{
		SetCurrentMana(FMath::Clamp(GetCurrentMana(), 0.f, GetMaxMana()));
	}
	
	Super::PostGameplayEffectExecute(Data);
}

void ULLL_PlayerCharacterAttributeSet::RecalculateEvasion(const float EvasionRateValue)
{
	if (EvasionRateValue != 0.0f)
	{
		SetEvasionFlag(FMath::RandRange(0.0f, 1.0f) <= EvasionRateValue);
	}
	else
	{
		SetEvasionFlag(false);
	}
}
