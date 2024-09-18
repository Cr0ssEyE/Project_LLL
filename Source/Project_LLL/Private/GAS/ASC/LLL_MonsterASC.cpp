// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC/LLL_MonsterASC.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
ULLL_MonsterASC::ULLL_MonsterASC()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void ULLL_MonsterASC::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<ILLL_KnockBackInterface>(GetAvatarActor()))
	{
		RegisterGameplayTagEvent(TAG_GAS_MONSTER_FALLABLE).AddUObject(this, &ULLL_MonsterASC::OnFallableTagAdded);
	}

	if (ALLL_MonsterBase* OwnerMonster = Cast<ALLL_MonsterBase>(GetAvatarActor()))
	{
		OwnerMonster->CharacterDeadDelegate.AddDynamic(this, &ULLL_MonsterASC::ClearAllTimer);
	}
}

FActiveGameplayEffectHandle ULLL_MonsterASC::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey)
{
	CheckAbnormalEffect(GameplayEffect);
	
	return Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);
}

void ULLL_MonsterASC::OnFallableTagAdded(const FGameplayTag Tag, int32 count)
{
	if (Tag == TAG_GAS_MONSTER_FALLABLE && count > 0 && Cast<ILLL_KnockBackInterface>(GetAvatarActor()))
	{
		if (TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MONSTER_FALLABLE)))
		{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (DebugGameInstance->CheckMonsterHitCheckDebug() || DebugGameInstance->CheckMonsterCollisionDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("낙하 상태로 전환 %s"), *Tag.GetTagName().ToString()));
				}
			}
#endif
		}
	}
}

void ULLL_MonsterASC::CheckAbnormalEffect(const FGameplayEffectSpec& GameplayEffectSpec)
{
	ALLL_MonsterBase* Monster = CastChecked<ALLL_MonsterBase>(GetAvatarActor());
	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(Player))
	{
		return;
	}

	const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(PlayerASC->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));
	
	if (GameplayEffectSpec.Def->GetAssetTags().HasTag(TAG_GAS_BLEEDING))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(BleedingTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);
		}

		Monster->UpdateBleedingVFX(true);

		if (Monster->GetBleedingStack() < 5)
		{
			Monster->SetBleedingStack(Monster->GetBleedingStack() + 1);
			Monster->UpdateStackVFX(Monster->GetBleedingStack(), 5);
		}
		
		GetWorld()->GetTimerManager().SetTimer(BleedingTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, Monster]{
			if (!IsValid(Monster))
			{
				return;
			}

			RemoveActiveEffectsWithGrantedTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING));

			Monster->UpdateBleedingVFX(false);

			Monster->SetBleedingStack(0);
			Monster->UpdateStackVFX(Monster->GetBleedingStack(), 5);
		}), AbnormalStatusAttributeSet->GetBleedingStatusDuration(), false);
	}
}

void ULLL_MonsterASC::ClearAllTimer(ALLL_BaseCharacter* Character)
{
	GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);

	BleedingTimerHandle.Invalidate();
}
