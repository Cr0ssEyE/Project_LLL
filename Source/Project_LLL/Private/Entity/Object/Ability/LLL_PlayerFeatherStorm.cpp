// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_PlayerFeatherStorm.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Object/Ability/LLL_PlayerFeatherStormAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_PlayerFeatherStorm::ALLL_PlayerFeatherStorm()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerFeatherStormDataAsset>(PATH_PLAYER_FEATHER_STORM_DATA, EAssertionLevel::Check);

	PlayerFeatherStormAttributeSet = CreateDefaultSubobject<ULLL_PlayerFeatherStormAttributeSet>(TEXT("PlayerFeatherStormAttributeSet"));
}

void ALLL_PlayerFeatherStorm::BeginPlay()
{
	Super::BeginPlay();

	PlayerFeatherStormDataAsset = Cast<ULLL_PlayerFeatherStormDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = PlayerFeatherStormAttributeSet;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerDashDebug())
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), OverlapCollisionBox->GetScaledBoxExtent(), FColor::Blue, false, AbilityObjectAttributeSet->GetDestroyTimer());
		}
	}
#endif
}

void ALLL_PlayerFeatherStorm::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginCursorOver();
	
	GetWorldTimerManager().SetTimer(KeepDamageHandle, FTimerDelegate::CreateWeakLambda(this, [&, OtherActor]{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(AbilityObjectDataAsset->DamageEffect, 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
			{
				UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *OtherActor->GetName())
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
			}
		}
	}), PlayerFeatherStormAttributeSet->GetDamageTimer(), true);
}

void ALLL_PlayerFeatherStorm::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	GetWorldTimerManager().ClearTimer(KeepDamageHandle);
}
