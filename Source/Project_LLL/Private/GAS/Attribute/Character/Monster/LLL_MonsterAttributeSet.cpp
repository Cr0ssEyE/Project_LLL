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
	bool Result = Super::PreGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		FGameplayTagContainer TagContainer(TAG_GAS_STATUS_BLEEDING);
		if (GetOwningAbilitySystemComponentChecked()->HasAnyMatchingGameplayTags(TagContainer))
		{
			CheckAbnormalStatus(Data);
		}
	}
	return Result;
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

		bool damaged = false;
		ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetOwningActor());
		if (GetCurrentShield() > 0)
		{
			SetCurrentShield(FMath::Clamp(GetCurrentShield() - GetReceiveDamage(), 0.f, GetMaxShield()));
		
			Monster->Damaged(Attacker, DOT);
			damaged = true;
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
				Monster->Damaged(Attacker, DOT);
				damaged = true;
			}
		}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterHitCheckDebug() && Cast<ALLL_MonsterBase>(GetOwningActor()) && damaged)
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
	float Damage = Data.EvaluatedData.Magnitude;
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(Player))
	{
		return;
	}

	Damage += Player->GetPlusOffencePower();
	
	UE_LOG(LogTemp, Log, TEXT("%s의 출혈 데미지 : %f"), *GetOwningActor()->GetName(), Damage)
	Data.EvaluatedData.Magnitude = Damage;

	UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();

	// 피의 갈증 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_VAMPIRE))
	{
		Player->VampireRecovery(Damage);
	}
}
