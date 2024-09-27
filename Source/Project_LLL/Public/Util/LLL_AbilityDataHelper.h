
#pragma once

#include "CoreMinimal.h"
#include "Constant/LLL_GameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Constant/LLL_AbilityRealNumbers.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_GameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Effect/LLL_GE_GiveAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Entity/Character/Player/LLL_InventoryWidget.h"
#include "UI/Entity/Character/Player/LLL_MainEruriaInfoWidget.h"

class PROJECT_LLL_API FLLL_AbilityDataHelper
{
public:
	// 이펙트의 상태이상 설정 관련,
	static void SetBleedingPeriodValue(const ALLL_PlayerBase* Player, ULLL_ExtendedGameplayEffect* Effect)
	{
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(PlayerASC->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));

		// 과다출혈 이누리아
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_EXCESSIVE_BLEEDING) && Player->GetWolfEnuriaCount() >= Player->GetExcessiveBleedingWolfEnuriaCheckCount())
		{
			Effect->SetPeriodValue(Player->GetExcessiveBleedingPeriod());
		}
		else
		{
			Effect->SetPeriodValue(AbnormalStatusAttributeSet->GetBleedingStatusPeriod());
		}
	}

	static bool CheckBleedingExplosion(const ALLL_PlayerBase* Player, ALLL_MonsterBase* Monster)
	{
		Monster->SetMaxBleedingStack(5);
		
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();

		// 혈우병 이누리아
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_BLEEDING_EXPLOSION))
		{
			Monster->SetMaxBleedingStack(3);
			if (Monster->GetBleedingStack() == Monster->GetMaxBleedingStack() - 1)
			{
				UAbilitySystemComponent* MonsterASC = Monster->GetAbilitySystemComponent();
				MonsterASC->RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING));
							
				Monster->SetBleedingStack(0);
				if (Monster->GetBleedingTrigger())
				{
					Monster->ToggleBleedingTrigger();
				}

				Monster->UpdateBleedingVFX(false);
				Monster->UpdateStackVFX(Monster->GetBleedingStack(), Monster->GetMaxBleedingStack());

				UE_LOG(LogTemp, Log, TEXT("혈우병 발동"))
				
				return true;
			}
		}
		return false;
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

	static void ApplyEnuriaEffect(const UWorld* World, TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, const int32 EffectID, TArray<const FAbilityDataTable*>& AbilityData, const bool bIsTest)
	{
		if (!IsValid(World))
		{
			ensure(false);
			return;
		}

		const FAbilityDataTable* CurrentAbilityData = new FAbilityDataTable;
		for (const auto TableElement : World->GetGameInstanceChecked<ULLL_GameInstance>()->GetAbilityDataTable())
		{
			if (TableElement->ID == EffectID)
			{
				CurrentAbilityData = TableElement;
				break;
			}
		}

		const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(World, 0));
		const ULLL_PlayerUIManager* PlayerUIManager = Player->GetPlayerUIManager();
		UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();

		if (!IsValid(PlayerUIManager) || !IsValid(PlayerASC))
		{
			ensure(false);
			return;
		}

		if (LoadedEffects.IsEmpty())
		{
			Player->GetGoldComponent()->IncreaseMoney(123);
			return;
		}
	
		UE_LOG(LogTemp, Log, TEXT("부여 된 플레이어 이펙트"));
		for (auto& LoadedEffect : LoadedEffects)
		{
			ULLL_ExtendedGameplayEffect* Effect = CastChecked<ULLL_ExtendedGameplayEffect>(LoadedEffect.Get()->GetDefaultObject());
			Effect->SetAbilityInfo(CurrentAbilityData);

			FGameplayEffectContextHandle EffectContextHandle = PlayerASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(Player);
			const FGameplayEffectSpecHandle EffectSpecHandle = PlayerASC->MakeOutgoingSpec(Effect->GetClass(), 1.0, EffectContextHandle);
			if(!EffectSpecHandle.IsValid())
			{
				continue;
			}

			for (const auto GottenAbilityArrayEffectHandle : GottenAbilityArrayEffectHandles(World))
			{
				const ULLL_ExtendedGameplayEffect* GottenEffect = CastChecked<ULLL_ExtendedGameplayEffect>(PlayerASC->GetActiveGameplayEffect(GottenAbilityArrayEffectHandle)->Spec.Def);
				if (CurrentAbilityData->TagID[1] == '1' && GottenEffect->GetAbilityData()->TagID[1] == '1')
				{
					PlayerASC->RemoveActiveGameplayEffect(GottenAbilityArrayEffectHandle);
					if (CurrentAbilityData->AbilityName != GottenEffect->GetAbilityData()->AbilityName)
					{
						AbilityData.Emplace(GottenEffect->GetAbilityData());
					}
					UE_LOG(LogTemp, Log, TEXT("사용 타입 이펙트 삭제"));
				}
				else if (CurrentAbilityData->AbilityName == GottenEffect->GetAbilityData()->AbilityName)
				{
					PlayerASC->RemoveActiveGameplayEffect(GottenAbilityArrayEffectHandle);
					UE_LOG(LogTemp, Log, TEXT("낮은 티어 이펙트 삭제"));
				}
			}

			// 단순 수치 변화는 여기에서 적용.
			const float MagnitudeValue1 = CurrentAbilityData->AbilityValue1 / static_cast<uint32>(CurrentAbilityData->Value1Type);
			const float MagnitudeValue2 = CurrentAbilityData->AbilityValue2 / static_cast<uint32>(CurrentAbilityData->Value2Type);
		
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, MagnitudeValue1);
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_2, MagnitudeValue2);
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_HUNDRED_VALUE_1, MagnitudeValue1 * 100.0f);
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_HUNDRED_VALUE_2, MagnitudeValue2 * 100.0f);
			PlayerASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		
			// 어빌리티 부여 계열
			if (ULLL_GE_GiveAbilityComponent* AbilitiesGameplayEffectComponent = &Effect->FindOrAddComponent<ULLL_GE_GiveAbilityComponent>())
			{
				for (const auto& AbilitySpecConfig : AbilitiesGameplayEffectComponent->GetAbilitySpecConfigs())
				{
					if (FGameplayAbilitySpec* Spec = PlayerASC->FindAbilitySpecFromClass(AbilitySpecConfig.Ability))
					{
						// EGameplayAbilityInstancingPolicy::InstancedPerActor로 설정된 어빌리티 한정 정상작동
						Cast<ULLL_PGA_RewardAbilityBase>(Spec->GetPrimaryInstance())->SetAbilityInfo(CurrentAbilityData);
						UE_LOG(LogTemp, Log, TEXT("스펙에 접근해서 값 바꾸기 시도"));
					}
				}
			}
		
			UE_LOG(LogTemp, Log, TEXT("- %s 부여"), *LoadedEffect.Get()->GetName());
			break;
		}

		// TODO: UI 관련 상호작용 구현.
		PlayerUIManager->GetInventoryWidget()->SetEnuriaInfo(CurrentAbilityData);
		PlayerUIManager->GetMainEnuriaWidget()->SetEnuriaInfo(CurrentAbilityData);

		// 테스트 중이 아니면 테이블에서 중복 보상 제거 후 가중치 재계산
		if (!bIsTest)
		{
			// 중첩 타입 이누리아일 경우 제거하지 않기
			if (CurrentAbilityData->TagID[0] != '1')
			{
				AbilityData.Remove(CurrentAbilityData);
			}
		}
	}

	static TArray<FActiveGameplayEffectHandle> GottenAbilityArrayEffectHandles(const UWorld* World)
	{
		const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(World, 0));
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		
		TArray<FActiveGameplayEffectHandle> EffectHandles;
		const TArray<FActiveGameplayEffectHandle> AllowEffectHandles = PlayerASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_ABILITY_NESTING_ALLOW));
		const TArray<FActiveGameplayEffectHandle> DenyEffectHandles = PlayerASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_ABILITY_NESTING_DENY));
		
		EffectHandles.Append(AllowEffectHandles);
		EffectHandles.Append(DenyEffectHandles);

		return EffectHandles;
	}

	static TArray<const FAbilityDataTable*> GottenAbilityArray(const UWorld* World)
	{
		TArray<const FAbilityDataTable*> GottenAbilityArray;
		
		const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(World, 0));
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		
		for (const auto EffectHandle : GottenAbilityArrayEffectHandles(World))
		{
			const ULLL_ExtendedGameplayEffect* ActiveEffect = CastChecked<ULLL_ExtendedGameplayEffect>(PlayerASC->GetActiveGameplayEffect(EffectHandle)->Spec.Def);
			GottenAbilityArray.Emplace(ActiveEffect->GetAbilityData());
		}

		return GottenAbilityArray;
	}
};
