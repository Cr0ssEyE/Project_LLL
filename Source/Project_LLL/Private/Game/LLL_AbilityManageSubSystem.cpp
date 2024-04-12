// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_AbilityManageSubSystem.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Constant/LLL_FilePath.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "GAS/LLL_ExtendedGameplayEffect.h"

ULLL_AbilityManageSubSystem::ULLL_AbilityManageSubSystem()
{
	
}

void ULLL_AbilityManageSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	LoadEffectsFromPath(PlayerGameplayEffects, PATH_PLAYER_EFFECTS);
	LoadEffectsFromPath(MonsterGameplayEffects, PATH_MONSTER_EFFECTS);
	LoadEffectsFromPath(ObjectGameplayEffects, PATH_OBJECT_EFFECTS);
	LoadEffectsFromPath(ShareableGameplayEffects, PATH_SHARE_EFFECTS);
}

void ULLL_AbilityManageSubSystem::Deinitialize()
{
	Super::Deinitialize();

}

void ULLL_AbilityManageSubSystem::LoadEffectsFromPath(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& Container, const FName PrimaryTypes)
{
	UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(PrimaryTypes, Assets);

	for (auto AssetData : Assets)
	{
		TSoftClassPtr<ULLL_ExtendedGameplayEffect> SoftPtr(Manager.GetPrimaryAssetPath(AssetData));
		Container.Emplace(SoftPtr);
	}
}

TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindEffectsByTag(EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching)
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> OwnerDataSet = GetDataSetByOwner(Owner);
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> FilteredList = FindEffectsFromDataSet(OwnerDataSet, EffectTag, TagHasMatching);
	
	for (auto Effect : FilteredList)
	{
		if (Effect.IsPending())
		{
			// Effect.LoadSynchronous();
		}
		ULLL_ExtendedGameplayEffect* EffectObject = Cast<ULLL_ExtendedGameplayEffect>(Effect->GetDefaultObject());
		if (EffectObject->GetOwnership() != Owner)
		{
			FilteredList.Remove(Effect);
		}
	}
	
	return FilteredList;
}

TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindAttributeAccessEffectsByTag(const EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching)
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> OwnerDataSet = GetDataSetByOwner(Owner);
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> FilteredList = FindEffectsFromDataSet(OwnerDataSet, EffectTag, TagHasMatching);
	
	for (auto Effect : FilteredList)
	{
		if (Effect.IsPending())
		{
			// Effect.LoadSynchronous();
		}
		ULLL_ExtendedGameplayEffect* EffectObject = Cast<ULLL_ExtendedGameplayEffect>(Effect->GetDefaultObject());
		if (EffectObject->GetOwnership() != Owner && EffectObject->GetAccessRange() == EEffectAccessRange::Ability)
		{
			FilteredList.Remove(Effect);
		}
	}
	
	return FilteredList;
}

TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindAbilityGrantEffectsByTag(const EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching)
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> OwnerDataSet = GetDataSetByOwner(Owner);
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> FilteredList = FindEffectsFromDataSet(OwnerDataSet, EffectTag, TagHasMatching);
	
	for (auto Effect : FilteredList)
	{
		if (Effect.IsPending())
		{
			// Effect.LoadSynchronous();
		}
		ULLL_ExtendedGameplayEffect* EffectObject = Cast<ULLL_ExtendedGameplayEffect>(Effect->GetDefaultObject());
		if (EffectObject->GetOwnership() != Owner && EffectObject->GetAccessRange() == EEffectAccessRange::Attribute)
		{
			FilteredList.Remove(Effect);
		}
	}
	
	return FilteredList;
}

TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& ULLL_AbilityManageSubSystem::GetDataSetByOwner(const EEffectOwnerType Owner)
{
	switch (Owner)
	{
	case EEffectOwnerType::Share:
		return ShareableGameplayEffects;
		
	case EEffectOwnerType::Player:
		return PlayerGameplayEffects;

	case EEffectOwnerType::Monster:
		return MonsterGameplayEffects;

	case EEffectOwnerType::Object:
		return ObjectGameplayEffects;

	default:
		checkNoEntry();
	}
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>* EmptyArray = new TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>;
	ensure(false);
	return *EmptyArray;
}

TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindEffectsFromDataSet(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& DataSet, const FGameplayTagContainer& EffectTag, bool HasMatching)
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> FilteredDataSet;

	for (auto Data : DataSet)
	{
		if (Data.IsPending())
		{
			// Data.LoadSynchronous();
		}
		
		ULLL_ExtendedGameplayEffect* EffectObject = Cast<ULLL_ExtendedGameplayEffect>(Data->GetDefaultObject());
		if(EffectObject->GetAssetTags().IsEmpty())
		{
			continue;
		}
		
		if (HasMatching)
		{
			if (EffectObject->GetAssetTags().HasAllExact(EffectTag))
			{
				FilteredDataSet.Emplace(Data);
			}
			continue;
		}
		
		if (EffectObject->GetAssetTags().HasAny(EffectTag))
		{
			FilteredDataSet.Emplace(Data);
		}
	}
	return FilteredDataSet;
}

