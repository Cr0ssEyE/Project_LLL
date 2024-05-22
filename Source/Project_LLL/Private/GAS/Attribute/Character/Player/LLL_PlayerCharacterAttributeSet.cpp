// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "Util/LLL_MathHelper.h"

ULLL_PlayerCharacterAttributeSet::ULLL_PlayerCharacterAttributeSet() :
TargetingCorrectionRadius(100.f)
{
	
}

void ULLL_PlayerCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetOwningActor());
	if (!IsValid(Player))
	{
		return;
	}
	
	if (Data.EvaluatedData.Attribute == GetReceiveDamageAttribute())
	{
		const uint32 DeclinedComboCount = FMath::FloorToInt(GetCurrentComboCount() * GetMultiplyComboCountWhenHit());
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckPlayerHitDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("플레이어 데미지 입음. : %f"), Data.EvaluatedData.Magnitude));
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("플레이어 데미지를 입어 콤보 감소. %d -> %d"), static_cast<int32>(GetCurrentComboCount()), DeclinedComboCount));
			}
		}
#endif
		
		SetCurrentComboCount(FMath::Clamp(DeclinedComboCount, 0.f, GetMaxComboCount()));
	}
	
	if (Data.EvaluatedData.Attribute == GetCurrentComboCountAttribute())
	{
		TryStartComboManagement(Data);
	}

	if (Data.EvaluatedData.Attribute == GetbIsComboTimerElapsedAttribute())
	{
		const uint32 DeclinedComboCount = FMath::FloorToInt(GetCurrentComboCount() * GetMultiplyComboCountPerTime());

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("시간 경과에 따른 플레이어 콤보 감소. %d -> %d"), static_cast<int32>(GetCurrentComboCount()), DeclinedComboCount));
			}
		}
#endif
		
		SetCurrentComboCount(FMath::Clamp(DeclinedComboCount, 0.f, GetMaxComboCount()));
		
		SetbIsComboTimerElapsed(0.f);
	}
	
	Super::PostGameplayEffectExecute(Data);
}

void ULLL_PlayerCharacterAttributeSet::TryStartComboManagement(const FGameplayEffectModCallbackData& Data)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("현재 콤보 값: %f"), GetCurrentComboCount()));
		}
	}
#endif
		
	SetCurrentComboCount(FMath::Clamp(GetCurrentComboCount(), 0.f, GetMaxComboCount()));
	
	UAbilitySystemComponent* OwnerASC = GetOwningAbilitySystemComponentChecked();
	const FGameplayTagContainer ComboManagementTag(TAG_GAS_COMBO_MANAGEMENT);
	OwnerASC->TryActivateAbilitiesByTag(ComboManagementTag);
}
	
