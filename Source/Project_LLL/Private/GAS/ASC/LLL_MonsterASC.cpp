// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC/LLL_MonsterASC.h"

#include "Constant/LLL_GameplayTags.h"
#include "Interface/LLL_KnockBackInterface.h"


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
}

void ULLL_MonsterASC::OnFallableTagAdded(const FGameplayTag Tag, int32 count)
{
	if (Tag == TAG_GAS_MONSTER_FALLABLE && count > 0 && Cast<ILLL_KnockBackInterface>(GetAvatarActor()))
	{
		if (TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MONSTER_FALLABLE)))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("낙하 상태로 전환 %s"), *Tag.GetTagName().ToString()));
		}
	}
}

void ULLL_MonsterASC::CheckAbnormalEffect(const FGameplayEffectSpec& GameplayEffectSpec)
{
	if (GameplayEffectSpec.Def->GetAssetTags().HasTag(TAG_GAS_BLEEDING))
	{
		TArray<FGameplayTag> EffectGrantTags = GameplayEffectSpec.Def->GetGrantedTags().GetGameplayTagArray();
		if (EffectGrantTags.Find(TAG_GAS_STATUS_BLEEDING_BASE_ATTACK) != INDEX_NONE)
		{
			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING_BASE_ATTACK));
		}
		
		if (EffectGrantTags.Find(TAG_GAS_STATUS_BLEEDING_CHASE_ATTACK) != INDEX_NONE)
		{
			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING_CHASE_ATTACK));
		}

		if (EffectGrantTags.Find(TAG_GAS_STATUS_BLEEDING_DASH_ATTACK) != INDEX_NONE)
		{
			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING_DASH_ATTACK));
		}
	}
}
