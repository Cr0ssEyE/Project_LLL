// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC/LLL_MonsterASC.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
ULLL_MonsterASC::ULLL_MonsterASC()
{
	PrimaryComponentTick.bCanEverTick = true;

}

FActiveGameplayEffectHandle ULLL_MonsterASC::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect,
	FPredictionKey PredictionKey)
{
	CheckAbnormalEffect(GameplayEffect);
	return Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);
}


// Called when the game starts
void ULLL_MonsterASC::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<ILLL_KnockBackInterface>(GetAvatarActor()))
	{
		RegisterGameplayTagEvent(TAG_GAS_MONSTER_FALLABLE).AddUObject(this, &ULLL_MonsterASC::OnFallableTagAdded);
	}
	
	RegisterGameplayTagEvent(TAG_GAS_MARK_STACK, EGameplayTagEventType::AnyCountChange).AddUObject(this, &ULLL_MonsterASC::OnMarkTagAdded);
}

void ULLL_MonsterASC::BeginDestroy()
{
	MarkTimerHandle.Invalidate();
	Super::BeginDestroy();
}

void ULLL_MonsterASC::OnFallableTagAdded(const FGameplayTag Tag, int32 count)
{
	if (Tag == TAG_GAS_MONSTER_FALLABLE && count > 0 && Cast<ILLL_KnockBackInterface>(GetAvatarActor()))
	{
		if (TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MONSTER_FALLABLE)))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("낙하 상태로 전환 %s"), *Tag.GetTagName().ToString()));
		}
	}
}

void ULLL_MonsterASC::OnMarkTagAdded(const FGameplayTag Tag, int32 count)
{
	if (Tag != TAG_GAS_MARK_STACK)
	{
		return;
	}

	if (!GetWorld() || !GetOwnerActor()->GetWorld())
	{
		return;
	}
	
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Character)
	{
		return;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(MarkTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(MarkTimerHandle);
	}
	
	const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(Cast<ALLL_PlayerBase>(Character)->GetAbilitySystemComponent()->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));
	
	if (GetTagCount(TAG_GAS_MARK_STACK) >= AbnormalStatusAttributeSet->GetMaxMarkStack())
	{
		SetTagMapCount(TAG_GAS_MARK_STACK, AbnormalStatusAttributeSet->GetMaxMarkStack());
	}
	
	if (!HasMatchingGameplayTag(TAG_GAS_STATUS_MARKED))
	{
		AddLooseGameplayTag(TAG_GAS_STATUS_MARKED);
	}

	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActor());
	Monster->UpdateMarkVFX(GetTagCount(TAG_GAS_MARK_STACK), AbnormalStatusAttributeSet->GetMaxMarkStack());
	
	GetWorld()->GetTimerManager().SetTimer(MarkTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]()
	{
		if (!Monster || !IsValid(this) || !GetWorld())
		{
			return;
		}
		
		SetTagMapCount(TAG_GAS_MARK_STACK,0);
		if (HasMatchingGameplayTag(TAG_GAS_STATUS_MARKED))
		{
			RemoveLooseGameplayTag(TAG_GAS_STATUS_MARKED);
		}
		Monster->UpdateMarkVFX(0, AbnormalStatusAttributeSet->GetMaxMarkStack());
	}), AbnormalStatusAttributeSet->GetMarkStatusDuration(), false);
}

void ULLL_MonsterASC::CheckAbnormalEffect(const FGameplayEffectSpec& GameplayEffectSpec)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActor());
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!Character)
	{
		return;
	}

	const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(Cast<ALLL_PlayerBase>(Character)->GetAbilitySystemComponent()->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));
	
	if (GameplayEffectSpec.Def->GetAssetTags().HasTag(TAG_GAS_BLEEDING))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(BleedingTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);
		}

		Monster->UpdateBleedingVFX(true);
		TArray<FGameplayTag> EffectGrantTags = GameplayEffectSpec.Def->GetGrantedTags().GetGameplayTagArray();
		FGameplayTag EffectBleedingTag;
		if (EffectGrantTags.Find(TAG_GAS_STATUS_BLEEDING_BASE_ATTACK) != INDEX_NONE)
		{
			EffectBleedingTag = TAG_GAS_STATUS_BLEEDING_BASE_ATTACK;
			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING_BASE_ATTACK));
		}
		
		if (EffectGrantTags.Find(TAG_GAS_STATUS_BLEEDING_CHASE_ATTACK) != INDEX_NONE)
		{
			EffectBleedingTag = TAG_GAS_STATUS_BLEEDING_CHASE_ATTACK;
			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING_CHASE_ATTACK));
		}

		if (EffectGrantTags.Find(TAG_GAS_STATUS_BLEEDING_DASH_ATTACK) != INDEX_NONE)
		{
			EffectBleedingTag = TAG_GAS_STATUS_BLEEDING_DASH_ATTACK;
			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING_DASH_ATTACK));
		}
		
		GetWorld()->GetTimerManager().SetTimer(BleedingTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]()
		{
			if (!Monster)
			{
				return;
			}

			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(EffectBleedingTag));
			Monster->UpdateBleedingVFX(false);
		}), AbnormalStatusAttributeSet->GetBleedingStatusDuration(), false);
	}
}
