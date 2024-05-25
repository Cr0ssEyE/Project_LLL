// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/RangedMonster/LLL_MGA_SpawnThrownObject.h"

#include "Entity/Character/Monster/Ranged/Base/LLL_RangedMonster.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/Base/LLL_ThrownObjectAttributeSet.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
#include "Util/LLL_MathHelper.h"

void ULLL_MGA_SpawnThrownObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_RangedMonster* RangedMonster = CastChecked<ALLL_RangedMonster>(GetAvatarActorFromActorInfo());
	const ULLL_RangedMonsterDataAsset* RangedMonsterDataAsset = CastChecked<ULLL_RangedMonsterDataAsset>(RangedMonster->GetCharacterDataAsset());
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(RangedMonster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

	ALLL_ThrownObject* ThrownObject = CastChecked<ALLL_ThrownObject>(RangedMonster->GetObjectPoolingComponent()->GetActor(RangedMonsterDataAsset->ThrownObjectClass));

	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerBase->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	if (IsValid(PlayerBase))
	{
		const FVector PredictedLocation = FLLL_MathHelper::GetPredictedLocation(RangedMonster, PlayerBase, PlayerAttributeSet->GetMoveSpeed(), MonsterAttributeSet->GetMonsterData2());
		const FVector StartLocation = RangedMonster->GetActorLocation();
		const FVector PredictedDirection = (PredictedLocation - StartLocation).GetSafeNormal();
		const FRotator PredictedRotation = FRotationMatrix::MakeFromX(PredictedDirection).Rotator();
	
		ThrownObject->SetActorLocationAndRotation(StartLocation, PredictedRotation);
		ThrownObject->CustomTimeDilation = RangedMonster->CustomTimeDilation;
		ThrownObject->Throw(RangedMonster, PlayerBase, MonsterAttributeSet->GetMonsterData1());

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
