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
	
	LoadEffectsFromPath(PlayerGameplayEffects, PATH_PLAYER_EFFECTS_FOLDER);
	LoadEffectsFromPath(MonsterGameplayEffects, PATH_MONSTER_EFFECTS_FOLDER);
	LoadEffectsFromPath(ObjectGameplayEffects, PATH_OBJECT_EFFECTS_FOLDER);
	LoadEffectsFromPath(ShareableGameplayEffects, PATH_SHARE_EFFECTS_FOLDER);
}

void ULLL_AbilityManageSubSystem::Deinitialize()
{
	Super::Deinitialize();

}

void ULLL_AbilityManageSubSystem::LoadEffectsFromPath(TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>>& Container, const FName Path)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;
	
	AssetRegistryModule.Get().GetAssetsByPath(Path, AssetDataList, true);
	for (auto AssetData : AssetDataList)
	{
		UClass* AssetClass = LoadObject<UClass>(nullptr, *FString::Printf(TEXT("Blueprint'%s_C'"), *AssetData.GetSoftObjectPath().GetAssetPathString()));
		if (AssetClass)
		{
			if (Cast<ULLL_ExtendedGameplayEffect>(AssetClass->GetDefaultObject()))
			{
				Container.Emplace(AssetClass);
			}
		}
	}
}

TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindEffectsFromDataSet(TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>>& DataSet, const FGameplayTagContainer& EffectTag, bool HasMatching)
{
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> FilteredDataSet;
	for (auto Data : DataSet)
	{
		if(Data.GetDefaultObject()->GetAssetTags().IsEmpty())
		{
			continue;
		}
		
		if (HasMatching)
		{
			if (Data.GetDefaultObject()->GetAssetTags().HasAllExact(EffectTag))
			{
				FilteredDataSet.Emplace(Data);
			}
			continue;
		}
		
		if (Data.GetDefaultObject()->GetAssetTags().HasAny(EffectTag))
		{
			FilteredDataSet.Emplace(Data);
		}
	}
	return FilteredDataSet;
}

TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindEffectsByTag(EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching)
{
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> OwnerDataSet = GetDataSetByOwner(Owner);
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> FilteredList = FindEffectsFromDataSet(OwnerDataSet, EffectTag, TagHasMatching);
	
	for (auto Effect : FilteredList)
	{
		if (Effect.GetDefaultObject()->GetOwnership() != Owner)
		{
			FilteredList.Remove(Effect);
		}
	}
	
	return FilteredList;
}

TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindAttributeAccessEffectsByTag(const EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching)
{
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> OwnerDataSet = GetDataSetByOwner(Owner);
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> FilteredList = FindEffectsFromDataSet(OwnerDataSet, EffectTag, TagHasMatching);
	
	for (auto Effect : FilteredList)
	{
		if (Effect.GetDefaultObject()->GetOwnership() != Owner && Effect.GetDefaultObject()->GetAccessRange() == EEffectAccessRange::Ability)
		{
			FilteredList.Remove(Effect);
		}
	}
	
	return FilteredList;
}

TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> ULLL_AbilityManageSubSystem::FindAbilityGrantEffectsByTag(const EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, bool TagHasMatching)
{
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> OwnerDataSet = GetDataSetByOwner(Owner);
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>> FilteredList = FindEffectsFromDataSet(OwnerDataSet, EffectTag, TagHasMatching);
	
	for (auto Effect : FilteredList)
	{
		if (Effect.GetDefaultObject()->GetOwnership() != Owner && Effect.GetDefaultObject()->GetAccessRange() == EEffectAccessRange::Attribute)
		{
			FilteredList.Remove(Effect);
		}
	}
	
	return FilteredList;
}

TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>>& ULLL_AbilityManageSubSystem::GetDataSetByOwner(const EEffectOwnerType Owner)
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
	TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>>* EmptyArray = new TArray<TSubclassOf<ULLL_ExtendedGameplayEffect>>;
	return *EmptyArray;
}
