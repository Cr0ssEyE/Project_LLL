
#pragma once

#include "CoreMinimal.h"
#include "Constant/LLL_GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Constant/LLL_AbilityRealNumbers.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "Game/LLL_GameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "GAS/Effect/LLL_GE_GiveAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Entity/Character/Player/LLL_InventoryWidget.h"
#include "UI/Entity/Character/Player/LLL_MainEruriaInfoWidget.h"

class PROJECT_LLL_API FLLL_AbilityDataHelper
{
public:
	// 이펙트의 상태이상 설정 관련,
	static void SetBleedingStatusAbilityDuration(const ALLL_PlayerBase* Player, const TSharedPtr<FGameplayEffectSpec>& EffectSpec)
	{
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(PlayerASC->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));

		// EffectSpec->bDurationLocked = false 하는 이유는 코드 외적인 부분에서 
		EffectSpec->bDurationLocked = false;
		EffectSpec->SetDuration(AbnormalStatusAttributeSet->GetBleedingStatusDuration(), true);
		EffectSpec->Period = AbnormalStatusAttributeSet->GetBleedingStatusPeriod();
		UE_LOG(LogTemp, Log, TEXT("%f Period 값 변경"), EffectSpec->GetPeriod());
	}

	static bool SpawnAbilityObject(const ULLL_CharacterGameplayAbilityBase* OwnerAbility, const TSubclassOf<ALLL_AbilityObject>& AbilityObjectClass, FGameplayEventData EventData = FGameplayEventData(), const EEffectApplyTarget AbilityObjectLocationTarget = EEffectApplyTarget::Self, const FVector& OffsetLocation = FVector::ZeroVector, const FRotator& OffsetRotator = FRotator::ZeroRotator)
	{
		UWorld* World = OwnerAbility->GetWorld();
		if (!World)
		{
			return false;	
		}
		
		FTransform SpawnTransform = FTransform::Identity;
		if (AbilityObjectLocationTarget == EEffectApplyTarget::Self)
		{
			SpawnTransform = OwnerAbility->GetAvatarActorFromActorInfo()->GetActorTransform();
		}
		else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(EventData.TargetData, 0))
		{
			SpawnTransform = EventData.TargetData.Data[0]->GetActors()[0]->GetActorTransform();
		}
		SpawnTransform.SetLocation(SpawnTransform.GetLocation() + SpawnTransform.GetRotation().RotateVector(OffsetLocation));
		SpawnTransform.SetRotation(SpawnTransform.GetRotation() * OffsetRotator.Quaternion());
	
		ALLL_AbilityObject* AbilityObject = World->SpawnActorDeferred<ALLL_AbilityObject>(AbilityObjectClass, SpawnTransform);
		if (const ULLL_PGA_RewardAbilityBase* RewardAbility = Cast<ULLL_PGA_RewardAbilityBase>(OwnerAbility))
		{
			AbilityObject->SetAbilityInfo(RewardAbility->GetAbilityData(), RewardAbility->GetAbilityLevel());
		}
		AbilityObject->SetOwner(OwnerAbility->GetAvatarActorFromActorInfo());
		AbilityObject->FinishSpawning(SpawnTransform);

		if (AbilityObject)
		{
			return true;
		}
		return false;
	}

	static TArray<const FAbilityDataTable*> ApplyEruriaEffect(UWorld* World, TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, int32 EffectID)
	{
		TArray<const FAbilityDataTable*> EqualAbilities;
		if (!IsValid(World))
		{
			ensure(false);
			return EqualAbilities;
		}
		
		ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(World, 0));
		const ULLL_PlayerUIManager* PlayerUIManager = Player->GetPlayerUIManager();
		UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
		
		if (!IsValid(PlayerUIManager) || !IsValid(ASC))
		{
			ensure(false);
			return EqualAbilities;
		}
		
		if (LoadedEffects.IsEmpty())
		{
			Player->GetGoldComponent()->IncreaseMoney(123);
			return EqualAbilities;
		}

		const FAbilityDataTable* CurrentAbilityData = new FAbilityDataTable;
		TArray<const FAbilityDataTable*> GlobalAbilityDataTable = World->GetGameInstanceChecked<ULLL_GameInstance>()->GetAbilityDataTable();
		for (const auto TableElement : GlobalAbilityDataTable)
		{
			if (TableElement->ID == EffectID)
			{
				CurrentAbilityData = TableElement;
				break;
			}
		}
		
		bool IsCommonEffect = true;
		
		UE_LOG(LogTemp, Log, TEXT("부여 된 플레이어 이펙트"));
		for (auto& LoadedEffect : LoadedEffects)
		{
			ULLL_ExtendedGameplayEffect* Effect = CastChecked<ULLL_ExtendedGameplayEffect>(LoadedEffect.Get()->GetDefaultObject());
			Effect->SetAbilityInfo(CurrentAbilityData);
		
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(Player);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Effect->GetClass(), 1.0, EffectContextHandle);
			if(!EffectSpecHandle.IsValid())
			{
				continue;
			}
			
			const FGameplayTagContainer TagContainer = Effect->GetAssetTags();
			if (TagContainer.HasTag(TAG_GAS_ABILITY_PART) && !TagContainer.HasTagExact(TAG_GAS_ABILITY_PART_COMMON))
			{
				// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("커먼 이펙트 아님")));
				IsCommonEffect = false;
				TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffectsWithAllTags(TagContainer);
				for (const auto EffectHandle : EffectHandles)
				{
					const ULLL_ExtendedGameplayEffect* ActiveEffect = Cast<ULLL_ExtendedGameplayEffect>(ASC->GetActiveGameplayEffect(EffectHandle)->Spec.Def);
					if (!IsValid(ActiveEffect))
					{
						continue;
					}
					
					if (CurrentAbilityData->AbilityPart == ActiveEffect->GetAbilityData()->AbilityPart)
					{
						ASC->RemoveActiveGameplayEffect(EffectHandle);
						for (auto GameplayTag : TagContainer.GetGameplayTagArray())
						{
							UE_LOG(LogTemp, Log, TEXT("- %s 태그를 가진 이펙트 삭제"), *GameplayTag.ToString());
						}
					}
				}
			}
		
			// 단순 수치 변화는 여기에서 적용.
			float ChangeableValue = CurrentAbilityData->AbilityValue / static_cast<uint32>(CurrentAbilityData->AbilityValueType);
			const float UnChangeableValue = CurrentAbilityData->UnchangeableValue;
			
			if (!EffectSpecHandle.Data->Def->Modifiers.IsEmpty())
			{
				switch (EffectSpecHandle.Data->Def->Modifiers[0].ModifierOp)
				{
				case EGameplayModOp::Multiplicitive:
					++ChangeableValue;
					break;
				default: // Add, Divide, Max, Override
					break;
				}
			}
			
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_CHANGEABLE_VALUE, ChangeableValue);
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_UNCHANGEABLE_VALUE, UnChangeableValue);
			
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			UE_LOG(LogTemp, Log, TEXT("- %s 부여"), *LoadedEffect.Get()->GetName());
			
			// 어빌리티 부여 계열
			if (ULLL_GE_GiveAbilityComponent* AbilitiesGameplayEffectComponent = &Effect->FindOrAddComponent<ULLL_GE_GiveAbilityComponent>())
			{
				for (const auto& AbilitySpecConfig : AbilitiesGameplayEffectComponent->GetAbilitySpecConfigs())
				{
					if (const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilitySpecConfig.Ability))
					{
						// EGameplayAbilityInstancingPolicy::InstancedPerActor로 설정된 어빌리티 한정 정상작동
						Cast<ULLL_PGA_RewardAbilityBase>(Spec->GetPrimaryInstance())->SetAbilityInfo(CurrentAbilityData);
						UE_LOG(LogTemp, Log, TEXT("스펙에 접근해서 값 바꾸기 시도"));
					}
				}
			}
		
			const FGameplayEventData PayLoadData;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_GAS_ABILITY_PART_GRANT, PayLoadData);
			break;
		}
		
		// TODO: UI 관련 상호작용 구현.
		if (IsCommonEffect)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("커먼 이펙트 획득")));
			PlayerUIManager->GetInventoryWidget()->SetEruriaInfo(CurrentAbilityData);
		}
		else
		{
			PlayerUIManager->GetMainEruriaWidget()->SetEruriaInfo(CurrentAbilityData);
		}
		
		uint8 Count = 2;
		for (auto Data : GlobalAbilityDataTable)
		{
			// 전설 이누리아의 경우 For문 중단
			if (CurrentAbilityData->ID % 10000 > ABILITY_RANK_LEGEND)
			{
				EqualAbilities.Emplace(Data);
				break;
			}
			
			if (Data->ID % ABILITY_INFO_ID == CurrentAbilityData->ID % ABILITY_INFO_ID)
			{
				EqualAbilities.Emplace(Data);
				--Count;
				if (!Count)
				{
					break;
				}
			}
		}

		return EqualAbilities;
	}
};
