// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/RangedMonster/LLL_MGA_SpawnThrownObject.h"

#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_ThrownObject.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Character/Monster/RangedMonster/LLL_RangedMonsterAttributeSet.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/Object/ThrownObject/LLL_ThrownObjectAttributeSet.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
#include "Util/LLL_MathHelper.h"

void ULLL_MGA_SpawnThrownObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_RangedMonster* RangedMonster = CastChecked<ALLL_RangedMonster>(GetAvatarActorFromActorInfo());
	const ULLL_RangedMonsterDataAsset* RangedMonsterDataAsset = CastChecked<ULLL_RangedMonsterDataAsset>(RangedMonster->GetCharacterDataAsset());
	const ULLL_RangedMonsterAttributeSet* RangedMonsterAttributeSet = CastChecked<ULLL_RangedMonsterAttributeSet>(RangedMonster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_RangedMonsterAttributeSet::StaticClass()));

	ALLL_ThrownObject* ThrownObject = CastChecked<ALLL_ThrownObject>(RangedMonster->GetObjectPoolingComponent()->GetActor(RangedMonsterDataAsset->ThrownObjectClass));

	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerBase->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	if (IsValid(PlayerBase))
	{
		const FVector PredictedLocation = FLLL_MathHelper::GetPredictedLocation(RangedMonster, PlayerBase, PlayerAttributeSet->GetMoveSpeed(), RangedMonsterAttributeSet->GetPredictionRate());
		const FVector StartLocation = RangedMonster->GetActorLocation();
		const FVector PredictedDirection = (PredictedLocation - StartLocation).GetSafeNormal();
		const FRotator PredictedRotation = FRotationMatrix::MakeFromX(PredictedDirection).Rotator();
	
		ThrownObject->SetActorLocationAndRotation(StartLocation, PredictedRotation);
		ThrownObject->Throw(RangedMonster);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterHitCheckDebug())
			{
				DrawDebugLine(GetWorld(), StartLocation, PredictedLocation, FColor::Yellow, false, 1.f);
			}
		}
#endif
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
