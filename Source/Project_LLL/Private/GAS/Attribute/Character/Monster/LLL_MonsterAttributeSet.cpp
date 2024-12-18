// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"
#include "Entity/Character/Monster/DPSTester/LLL_DPSTester.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Game/LLL_FallOutSubsytem.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_MonsterAttributeSet::ULLL_MonsterAttributeSet()
{
	
}

bool ULLL_MonsterAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		CheckAbnormalStatus(Data);
	}
	
	return Super::PreGameplayEffectExecute(Data);
}

void ULLL_MonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());
		if (Monster->CheckCharacterIsDead())
		{
			return;
		}
		
		ALLL_BaseCharacter* Attacker = Cast<ALLL_BaseCharacter>(Data.EffectSpec.GetEffectContext().Get()->GetInstigator());
		if (IsValid(Attacker))
		{
			const bool DOT = Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;

			bool Damaged;

			SetReceiveDamage(GetReceiveDamage() * GetReceiveDamageRate());
			SetReceiveDamage(FMath::Floor(GetReceiveDamage()));
			if (Cast<ALLL_DPSTester>(Monster))
			{
				Monster->Damaged(Attacker, DOT, GetReceiveDamage());
				Damaged = true;
			}
			else
			{
				if (GetCurrentShield() > 0)
				{
					SetCurrentShield(FMath::Clamp(GetCurrentShield() - GetReceiveDamage(), 0.f, GetMaxShield()));
		
					Monster->Damaged(Attacker, DOT, GetReceiveDamage());
					Damaged = true;
				}
				else
				{
					SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetReceiveDamage(), 0.f, GetMaxHealth()));

					if (GetCurrentHealth() == 0)
					{
						Monster->Dead();
						Damaged = true;
					}
					else
					{
						Monster->Damaged(Attacker, DOT, GetReceiveDamage());
						Damaged = true;
					}
				}

				if (Data.EffectSpec.Def->GetAssetTags().HasTag(TAG_GAS_STUN_HARD) && Damaged)
				{
					if (Cast<ALLL_BossMonster>(Monster))
					{
						if (GetCurrentHealth() != 0)
						{
							Monster->Stun();
						}
					}
					else
					{
						FVector HitNormal = (Monster->GetActorLocation() - Attacker->GetActorLocation()).GetSafeNormal2D();
						if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Attacker))
						{
							if (!Player->CheckAttackIsRange())
							{
								HitNormal = Player->GetKnockBackDirection();
								GetWorld()->GetGameInstance()->GetSubsystem<ULLL_FallOutSubsystem>()->FallOutBegin(Monster, HitNormal, Monster->GetActorLocation());
							}
							else
							{
								Monster->Stun();
							}
						}
						else
						{
							GetWorld()->GetGameInstance()->GetSubsystem<ULLL_FallOutSubsystem>()->FallOutBegin(Monster, HitNormal, Monster->GetActorLocation());
						}
					}
				}
			}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (ProtoGameInstance->CheckMonsterHitCheckDebug() && Cast<ALLL_MonsterBase>(GetOwningActor()) && Damaged)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("%s가 데미지 입음: %f"), *Monster->GetName(), Data.EvaluatedData.Magnitude));
				}
			}
#endif
		}
	}
	
	Super::PostGameplayEffectExecute(Data);
}

void ULLL_MonsterAttributeSet::CheckAbnormalStatus(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EffectSpec.Def->GetAssetTags().HasTag(TAG_GAS_BLEEDING))
	{
		ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!IsValid(Player))
		{
			return;
		}
		
		const ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		
		float Damage = Data.EvaluatedData.Magnitude;
		Damage *= Monster->GetBleedingStack();
		Damage = FLLL_AbilityDataHelper::CalculateOffencePower(Damage, Player);

		// 과다출혈 이누리아
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_EXCESSIVE_BLEEDING) && Monster->GetBleedingStack() == Monster->GetMaxBleedingStack())
		{
			Damage += Player->GetExcessiveBleedingOffencePowerPlus();
		}
	
		UE_LOG(LogTemp, Log, TEXT("%s의 출혈 데미지 : %f"), *Monster->GetName(), Damage)
		Data.EvaluatedData.Magnitude = Damage;

		// 피의 갈증 이누리아
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_VAMPIRE))
		{
			Player->VampireRecovery(Damage);
		}
	}
}
