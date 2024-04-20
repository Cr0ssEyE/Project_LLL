// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Entity/Object/LLL_BaseObject.h"

class PROJECT_LLL_API FLLL_ExecuteCueHelper
{
public:
	static void ExecuteCue(const AActor* SourceActor, FGameplayTag CueTag)
	{
		const ALLL_BaseCharacter* Character = Cast<ALLL_BaseCharacter>(SourceActor);
		const ALLL_BaseObject* Object = Cast<ALLL_BaseObject>(SourceActor);

		UAbilitySystemComponent* ASC = nullptr;
		if (IsValid(Character))
		{
			ASC = Character->GetAbilitySystemComponent();
		}
		else if (IsValid(Object))
		{
			ASC = Character->GetAbilitySystemComponent();
		}

		if (IsValid(ASC))
		{
			FGameplayEffectContextHandle CueContextHandle = ASC->MakeEffectContext();
			CueContextHandle.AddSourceObject(SourceActor);
			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			ASC->ExecuteGameplayCue(CueTag, CueParam);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : GAS 컴포넌트가 없습니다"), *SourceActor->GetName())
		}
	}
};
