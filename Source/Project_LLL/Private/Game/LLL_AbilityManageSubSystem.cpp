// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_AbilityManageSubSystem.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Constant/LLL_FilePath.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"

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

void ULLL_AbilityManageSubSystem::LoadEffectsFromPath(TArray<TSubclassOf<UGameplayEffect>>& Container, const FName Path)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;
	
	AssetRegistryModule.Get().GetAssetsByPath(Path, AssetDataList, true);
	for (auto AssetData : AssetDataList)
	{
		UBlueprint* Asset = Cast<UBlueprint>(AssetData.GetAsset());
		if (Asset)
		{
			Container.Emplace(Asset->GeneratedClass);
		}
	}
}

TArray<TSubclassOf<UGameplayEffect>> ULLL_AbilityManageSubSystem::FindEffectsByTag(TArray<TSubclassOf<UGameplayEffect>>& DataSet, const FGameplayTagContainer& EffectTag)
{
	TArray<TSubclassOf<UGameplayEffect>> FilteredDataSet;
	for (auto Data : DataSet)
	{
		if (Data.GetDefaultObject()->GetAssetTags().HasAny(EffectTag))
		{
			FilteredDataSet.Emplace(Data);
		}
	}
	return FilteredDataSet;
}

TArray<TSubclassOf<UGameplayEffect>> ULLL_AbilityManageSubSystem::FindPlayerEffectsByTag(ALLL_PlayerBase* TargetPlayer, const FGameplayTagContainer& EffectTag)
{
	TArray<TSubclassOf<UGameplayEffect>> FilteredList = FindEffectsByTag(PlayerGameplayEffects, EffectTag);
	return FilteredList;
}

TArray<TSubclassOf<UGameplayEffect>> ULLL_AbilityManageSubSystem::FindMonsterEffectsByTag(ALLL_MonsterBase* TargetMonster, const FGameplayTagContainer& EffectTag)
{
	TArray<TSubclassOf<UGameplayEffect>> FilteredList = FindEffectsByTag(MonsterGameplayEffects, EffectTag);
	return FilteredList;
}

TArray<TSubclassOf<UGameplayEffect>> ULLL_AbilityManageSubSystem::FindObjectEffectsByTag(ALLL_BaseObject* TargetObject,const FGameplayTagContainer& EffectTag)
{
	TArray<TSubclassOf<UGameplayEffect>> FilteredList = FindEffectsByTag(ObjectGameplayEffects, EffectTag);
	return FilteredList;
}

TArray<TSubclassOf<UGameplayEffect>> ULLL_AbilityManageSubSystem::FindShareableEffectsByTag(const FGameplayTagContainer& EffectTag)
{
	TArray<TSubclassOf<UGameplayEffect>> FilteredList = FindEffectsByTag(ShareableGameplayEffects, EffectTag);
	return FilteredList;
}
