// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/LLL_MGA_SpawnThrownObject.h"

#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_ThrownObject.h"
#include "Game/ProtoGameInstance.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"

void ULLL_MGA_SpawnThrownObject::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_RangedMonster* RangedMonster = CastChecked<ALLL_RangedMonster>(ActorInfo->AvatarActor);
	const ULLL_RangedMonsterDataAsset* RangedMonsterDataAsset = CastChecked<ULLL_RangedMonsterDataAsset>(RangedMonster->GetCharacterDataAsset());

	ALLL_ThrownObject* ThrownObject = Cast<ALLL_ThrownObject>(RangedMonster->GetObjectPoolingComponent()->GetActor(RangedMonsterDataAsset->ThrownObjectClass));
	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(ThrownObject) && IsValid(PlayerBase))
	{
		const float Speed = ThrownObject->GetSpeed();
		const float Distance = RangedMonster->GetDistanceTo(PlayerBase);
		const FVector StartLocation = RangedMonster->GetActorLocation();
		const FVector PredictedMove = PlayerBase->GetVelocity() * Distance / Speed;
		const FVector PredictedLocation = PlayerBase->GetActorLocation() + PredictedMove * RangedMonsterDataAsset->PredictionRate;
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
}
