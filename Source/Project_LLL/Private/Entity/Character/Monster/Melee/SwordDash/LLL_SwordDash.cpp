// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDashAIController.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/SwordDash/LLL_SwordDashAttributeSet.h"
#include "Util/LLLConstructorHelper.h"

ALLL_SwordDash::ALLL_SwordDash()
{
	CharacterAttributeSet = CreateDefaultSubobject<ULLL_SwordDashAttributeSet>(TEXT("SwordDashAttributeSet"));

	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_SwordDashDataAsset>(PATH_SWORD_DASH_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_SwordDashAIController::StaticClass();
}

void ALLL_SwordDash::Dash()
{
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	ASC->FindAllAbilitiesWithTags(AbilitySpecHandles, FGameplayTagContainer(TAG_GAS_SWORD_DASH_DASH));
	for (const auto AbilitySpecHandle : AbilitySpecHandles)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(AbilitySpecHandle))
		{
			if (ASC->TryActivateAbility(AbilitySpec->Handle))
			{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
				if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
				{
					if (ProtoGameInstance->CheckMonsterAttackDebug())
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 대시 수행"), *GetName()));
					}
				}
#endif
			}
		}
	}
}

void ALLL_SwordDash::BeginPlay()
{
	Super::BeginPlay();

	SwordDashDataAsset = Cast<ULLL_SwordDashDataAsset>(MeleeMonsterDataAsset);
}
