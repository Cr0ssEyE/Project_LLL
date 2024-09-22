// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_AbilityManageSubSystem.h"

#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Constant/LLL_FilePath.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"

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

	// AttributeSetInitter 사용 시 에디터 편의성용. 에디터에서는 최초 실행시 테이블 값만 읽어오고 이후에 값을 변경한 것을 적용하려면 재실행 해야 하는데 그거 보완

	TArray<FSoftObjectPath> DummyPaths;
	DummyPaths.Emplace(FSoftObjectPath(PATH_DUMMY_TABLE));
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->AddAttributeDefaultTables(TEXT("Dummy"), DummyPaths);
}

void ULLL_AbilityManageSubSystem::Deinitialize()
{
	Super::Deinitialize();
	
}

void ULLL_AbilityManageSubSystem::ASyncLoadEffectsByTag(FAsyncLoadEffectByTagDelegate Delegate, EEffectOwnerType Owner, const FGameplayTagContainer& EffectTag, const EEffectAccessRange AccessRange, bool TagHasMatching)
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> DataSet = GetDataSetByOwner(Owner);
	TArray<FSoftObjectPath> Paths;
	for (auto& Effect : DataSet)
	{
		Paths.Emplace(Effect.ToSoftObjectPath());
	}
	StreamableManager.RequestAsyncLoad(Paths, FStreamableDelegate::CreateWeakLambda(this, [=]()
	{
		TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> FilteredDataSet;
		for (auto Data : DataSet)
		{
			const ULLL_ExtendedGameplayEffect* EffectObject = CastChecked<ULLL_ExtendedGameplayEffect>(Data->GetDefaultObject());
			if (AccessRange != EEffectAccessRange::None && EffectObject->GetAccessRange() != AccessRange)
			{
				continue;
			}

			if(EffectObject->GetAssetTags().IsEmpty())
			{
				continue;
			}
		
			if (TagHasMatching)
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
		Delegate.Broadcast(FilteredDataSet, EffectTag);
	}));
}

void ULLL_AbilityManageSubSystem::ASyncLoadEffectsByID(FAsyncLoadEffectByIDDelegate Delegate, EEffectOwnerType Owner, int32 ID, const EEffectAccessRange AccessRange)
{
	FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
	TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> DataSet = GetDataSetByOwner(Owner);

	const FString OwnerName = StaticEnum<EEffectOwnerType>()->GetNameStringByValue(static_cast<int64>(Owner));
	UE_LOG(LogTemp, Log, TEXT("[ 로드된 %s 이펙트 수 : %d ]"), *OwnerName, DataSet.Num());
	
	TArray<FSoftObjectPath> Paths;
	for (auto& Effect : DataSet)
	{
		Paths.Emplace(Effect.ToSoftObjectPath());
	}

	if (Paths.IsEmpty())
	{
		Delegate.Broadcast(DataSet, ID);
		return;	
	}
	
	StreamableManager.RequestAsyncLoad(Paths, FStreamableDelegate::CreateWeakLambda(this, [=]()
	{
		TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>> FilteredDataSet;
		for (auto Data : DataSet)
		{
			const ULLL_ExtendedGameplayEffect* EffectObject = CastChecked<ULLL_ExtendedGameplayEffect>(Data->GetDefaultObject());
			if (AccessRange != EEffectAccessRange::None && EffectObject->GetAccessRange() != AccessRange)
			{
				continue;
			}

			for (auto EffectID : EffectObject->GetID())
			{
				if (EffectID == ID)
				{
					FilteredDataSet.Emplace(Data);
					break;
				}
			}
		}
		
		Delegate.Broadcast(FilteredDataSet, ID);
	}));
}

void ULLL_AbilityManageSubSystem::LoadEffectsFromPath(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& Container, const FName PrimaryTypes)
{
	const UAssetManager& Manager = UAssetManager::Get();
	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(PrimaryTypes, Assets);
	for (auto AssetData : Assets)
	{
		TSoftClassPtr<ULLL_ExtendedGameplayEffect> SoftPtr(Manager.GetPrimaryAssetPath(AssetData));
		if (SoftPtr.IsValid() || SoftPtr.IsPending())
		{
			SoftPtr.LoadSynchronous();
		}
		// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("추가 이펙트 %s"), *AssetData.PrimaryAssetName.ToString()));
		Container.Emplace(SoftPtr.Get());
	}
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
