// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_FeatherStorm.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Object/Ability/LLL_FeatherStormAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_FeatherStorm::ALLL_FeatherStorm()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_FeatherStormDataAsset>(PATH_FEATHER_STORM_DATA, EAssertionLevel::Check);

	FeatherStormAttributeSet = CreateDefaultSubobject<ULLL_FeatherStormAttributeSet>(TEXT("PlayerFeatherStormAttributeSet"));
}

void ALLL_FeatherStorm::BeginPlay()
{
	Super::BeginPlay();

	FeatherStormDataAsset = Cast<ULLL_FeatherStormDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = FeatherStormAttributeSet;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug())
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), OverlapCollisionBox->GetScaledBoxExtent(), FColor::Blue, false, AbilityObjectAttributeSet->GetDestroyTimer());
		}
	}
#endif
}

void ALLL_FeatherStorm::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	GetWorldTimerManager().SetTimer(KeepDamageHandle, FTimerDelegate::CreateWeakLambda(this, [&, OtherActor]{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(AbilityObjectDataAsset->DamageEffect, AbilityLevel, EffectContextHandle);
		const float OffencePower = AbilityData->AbilityValue + AbilityData->ChangeValue * (AbilityLevel - 1);

		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_CHANGEABLE_VALUE, OffencePower);
		if(EffectSpecHandle.IsValid())
		{
			if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
			{
				UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *OtherActor->GetName())
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
			}
		}
	}), FeatherStormAttributeSet->GetDamageTimer(), true);
}

void ALLL_FeatherStorm::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	GetWorldTimerManager().ClearTimer(KeepDamageHandle);
}
