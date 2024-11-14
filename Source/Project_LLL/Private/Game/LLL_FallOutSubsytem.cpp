// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_FallOutSubsytem.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MeshSocketName.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "DataAsset/Global/LLL_GlobalParameterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/LLL_GameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Ability/Character/Monster/LLL_MGA_SetFallableState.h"
#include "Util/LLL_MathHelper.h"

void ULLL_FallOutSubsystem::FallOutBegin(AActor* Actor, FVector HitNormal, FVector HitLocation)
{
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Actor);
	if (!IsValid(GetWorld()) || !IsValid(Monster))
	{
		return;
	}

	Monster->GetAbilitySystemComponent()->RemoveLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE, 99);
	Monster->CustomTimeDilation = 1.f;
	Monster->GetCharacterMovement()->Velocity = FVector::Zero();
	Monster->GetAbilitySystemComponent()->CancelAbility(Cast<UGameplayAbility>(ULLL_MGA_SetFallableState::StaticClass()->GetDefaultObject()));
	
	GetWorld()->GetWorldSettings()->SetTimeDilation(GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->FallEventTimeDilation);
	UNiagaraSystem* WallCrashNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->InvisibleWallCrashNiagaraSystem;
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallCrashNiagaraSystem, HitLocation, HitNormal.Rotation());
	NiagaraComponent->SetCustomTimeDilation(1.f / GetWorld()->GetWorldSettings()->TimeDilation);

	UNiagaraSystem* TrailNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->FallTrailNiagaraSystem;
	UNiagaraFunctionLibrary::SpawnSystemAttached(TrailNiagaraSystem, Monster->GetMesh(), SOCKET_CHEST, FVector::Zero(), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	
	FTimerHandle DilationTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DilationTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
		FallOutStart(Actor, HitNormal);
	}), GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->FallEventDuration * GetWorld()->GetWorldSettings()->TimeDilation, false);
}

void ULLL_FallOutSubsystem::FallOutStart(AActor* Actor, FVector HitNormal)
{
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Actor);
	if (!IsValid(GetWorld()) || !IsValid(Monster))
	{
		return;
	}
	
	Monster->CustomTimeDilation = 1.f;
	
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_OVERLAP_ALL);
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, HitNormal, Monster]
	{
		if (!IsValid(GetWorld()) || !IsValid(Monster))
		{
			return;
		}
		
		const float DefaultFallKnockBackPower = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->DefaultFallKnockBackPower;
		float StackedKnockBackPower = FMath::Max(Monster->GetKnockBackedPower() * 5.f, DefaultFallKnockBackPower);
		FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(HitNormal, StackedKnockBackPower);
		Monster->AddKnockBackVelocity(LaunchVelocity, -1.f);
	}));
}
