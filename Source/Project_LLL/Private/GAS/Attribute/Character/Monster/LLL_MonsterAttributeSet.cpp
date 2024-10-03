// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"

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
		AActor* Attacker = Data.EffectSpec.GetEffectContext().Get()->GetInstigator();
		const bool DOT = Data.EffectSpec.Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;
		if (const ALLL_ThrownObject* ThrownObject = Cast<ALLL_ThrownObject>(Attacker))
		{
			Attacker = ThrownObject->GetOwner();
		}

		bool Damaged = false;
		ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());

		SetReceiveDamage(FMath::Floor(GetReceiveDamage()));
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
			}
			else
			{
				Monster->Damaged(Attacker, DOT, GetReceiveDamage());
				Damaged = true;
			}
		}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterHitCheckDebug() && Cast<ALLL_MonsterBase>(GetOwningActor()) && Damaged)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("몬스터 데미지 입음. : %f"), Data.EvaluatedData.Magnitude));
			}
		}
#endif
	}
	
	Super::PostGameplayEffectExecute(Data);
}

void ULLL_MonsterAttributeSet::CheckAbnormalStatus(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EffectSpec.Def->GetAssetTags().HasTag(TAG_GAS_BLEEDING))
	{
		const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (!IsValid(Player))
		{
			return;
		}
		
		ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_BLEEDING_TRANSMISSION))
		{
			Monster->SetBleedingTransmissionOffencePower(Data.EvaluatedData.Magnitude);
		}
		
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
		float Damage = Data.EvaluatedData.Magnitude;
		Damage *= PlayerAttributeSet->GetAllOffencePowerRate();
		Damage *= Monster->GetBleedingStack();
		Damage += PlayerAttributeSet->GetAllOffencePowerPlus();

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
