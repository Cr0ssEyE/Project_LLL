// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/MeleeMonster/BombSkull/LLL_MGA_Explode.h"

#include "NiagaraFunctionLibrary.h"
#include "Constant/LLL_CollisionChannel.h"
#include "DataAsset/LLL_BombSkullDataAsset.h"
#include "DataAsset/Global/LLL_GlobalParameterDataAsset.h"
#include "Entity/Character/Monster/Melee/BombSkull/LLL_BombSkull.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Util/LLL_MathHelper.h"

void ULLL_MGA_Explode::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_BombSkull* BombSkull = CastChecked<ALLL_BombSkull>(GetAvatarActorFromActorInfo());
	const ULLL_BombSkullDataAsset* BombSkullDataAsset = CastChecked<ULLL_BombSkullDataAsset>(BombSkull->GetCharacterDataAsset());
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BombSkullDataAsset->ExplodeParticle, BombSkull->GetActorLocation(), BombSkull->GetActorRotation(), BombSkullDataAsset->ExplodeParticleScale, true);

	UAbilitySystemComponent* ASC = BombSkull->GetAbilitySystemComponent();
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ASC->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	const float ExplodeRadius = MonsterAttributeSet->GetBombSkullExplodeRadius();
	
	TArray<FHitResult> HitResults1;
	TArray<FHitResult> HitResults2;
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(BombSkull);
	
	GetWorld()->SweepMultiByChannel(
		HitResults1,
		BombSkull->GetActorLocation(),
		BombSkull->GetActorLocation(),
		FQuat::Identity,
		ECC_PLAYER,
		FCollisionShape::MakeSphere(ExplodeRadius),
		Params
		);
	
	GetWorld()->SweepMultiByChannel(
		HitResults2,
		BombSkull->GetActorLocation(),
		BombSkull->GetActorLocation(),
		FQuat::Identity,
		ECC_ENEMY,
		FCollisionShape::MakeSphere(ExplodeRadius),
		Params
		);

	TSet<ALLL_BaseCharacter*> Characters;
	for (auto HitResult : HitResults1 += HitResults2)
	{
		ALLL_BaseCharacter* Character = Cast<ALLL_BaseCharacter>(HitResult.GetActor());
		if (IsValid(Character) && !Character->CheckCharacterIsDead())
		{
			Characters.Emplace(Character);
		}
	}

	FColor DebugColor = FColor::Red;
	for (auto Character : Characters)
	{
		UAbilitySystemComponent* OtherASC = Character->GetAbilitySystemComponent();
		
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		EffectContextHandle.AddInstigator(BombSkull, BombSkull);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(BombSkullDataAsset->ExplodeDamageEffect, GetAbilityLevel(), EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, OtherASC);
		}

		if (ILLL_KnockBackInterface* KnockBackInterface = Cast<ILLL_KnockBackInterface>(Character))
		{
			// Todo : 낙사 넉백 처리 필요
		}
			
		DebugColor =  FColor::Green;
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			DrawDebugSphere(GetWorld(), BombSkull->GetActorLocation(), ExplodeRadius, 16, DebugColor, false, 2.0f);
		}
	}
#endif
	
	BombSkull->GetMesh()->SetHiddenInGame(true);
	BombSkull->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BombSkull->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
