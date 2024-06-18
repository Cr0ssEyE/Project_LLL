// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_AbilityObjectDataAsset.h"
#include "GameFramework/Character.h"
#include "GAS/Attribute/Object/Ability/Base/LLL_AbilityObjectAttributeSet.h"
#include "Util/LLL_MathHelper.h"

ALLL_AbilityObject::ALLL_AbilityObject()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OverlapCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Collision"));
	OverlapCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCollisionBox->SetCollisionProfileName(CP_PLAYER_ABILITY_OBJECT);
	OverlapCollisionBox->SetupAttachment(RootComponent);
}

void ALLL_AbilityObject::BeginPlay()
{
	Super::BeginPlay();

	AbilityObjectDataAsset = Cast<ULLL_AbilityObjectDataAsset>(BaseObjectDataAsset);

	SetOwner(GetWorld()->GetFirstPlayerController()->GetCharacter());
	OverlapCollisionBox->SetBoxExtent(AbilityObjectDataAsset->OverlapCollisionSize);

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]
	{
		FTimerHandle DestroyTimerHandle;
		GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
			Destroy();
		}), AbilityObjectAttributeSet->GetDestroyTimer(), false);
	}));
}

void ALLL_AbilityObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (AbilityData->AbilityValueType == EAbilityValueType::Fixed)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(AbilityObjectDataAsset->DamageEffect, AbilityLevel, EffectContextHandle);
		const float OffencePower = AbilityData->AbilityValue + AbilityData->ChangeValue * AbilityLevel;
		
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_CHANGEABLE_VALUE, OffencePower);
		if(EffectSpecHandle.IsValid())
		{
			if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor))
			{
				UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *OtherActor->GetName())
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("어빌리티 오브젝트 스폰 과정에서 능력 수치가 Percent로 넘어오고 있습니다"))
	}
}
