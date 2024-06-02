// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/SkillSystem/LLL_PGA_Skill_BulletTime.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/ASC/LLL_BaseASC.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerSkillAttributeSet.h"
#include "Interface/LLL_KnockBackInterface.h"

ULLL_PGA_Skill_BulletTime::ULLL_PGA_Skill_BulletTime()
{
	SkillDuration = 0.f;
	WorldDecelerationRate = 0.f;
	PlayerAccelerationRate = 0.f;
}

void ULLL_PGA_Skill_BulletTime::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ULLL_PlayerSkillAttributeSet* PlayerSkillAttributeSet = Cast<ULLL_PlayerSkillAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerSkillAttributeSet::StaticClass()));
	if (IsValid(PlayerSkillAttributeSet) && IsValid(BulletTimeActivateSequence))
	{
		ActorSpawnedDelegateHandle = GetWorld()->AddOnActorSpawnedHandler(WorldActorSpawnedDelegate.CreateUObject(this, &ULLL_PGA_Skill_BulletTime::OnBulletTimeEffectedActorSpawnCheck));
	
		SkillDuration = PlayerSkillAttributeSet->GetBulletTimeDuration();
		WorldDecelerationRate = PlayerSkillAttributeSet->GetBulletTimeWorldDecelerationRate();
		
		if (!IsValid(BulletTimeActivateSequenceActor))
		{
			BulletTimeActivateSequenceActor = GetWorld()->SpawnActorDeferred<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FTransform::Identity);
			FMovieSceneSequencePlaybackSettings Settings;
			Settings.bAutoPlay = false;
			Settings.bHideHud = false;
			BulletTimeActivateSequenceActor->PlaybackSettings = Settings;
			BulletTimeActivateSequenceActor->SetSequence(BulletTimeActivateSequence);
			BulletTimeActivateSequenceActor->FinishSpawning(FTransform::Identity);
		}

		TraceBulletTimeEffectedActors();
		FGameplayEventData EventData;
		EventData.InstigatorTags.AddTag(TAG_GAS_PLAYER_BULLET_TIME_START);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TAG_GAS_PLAYER_BULLET_TIME_START, EventData);
		BulletTimeActivateSequenceActor->GetSequencePlayer()->Play();

		GetWorld()->GetTimerManager().SetTimer(AbilityDurationTimerHandle, this, &ULLL_PGA_Skill_BulletTime::BulletTimeEndedCallBack, SkillDuration, false);
	}
	else
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckPlayerSkillDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("불릿타임 스킬 발동 취소됨. 시퀀스 또는 어트리뷰트 셋 접근 유효하지 않음")));
			}
		}
#endif
		
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void ULLL_PGA_Skill_BulletTime::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (bWasCancelled)
	{
		BulletTimeEndedCallBack();
	}

	FGameplayEventData EventData;
	EventData.InstigatorTags.AddTag(TAG_GAS_PLAYER_BULLET_TIME_END);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TAG_GAS_PLAYER_BULLET_TIME_END, EventData);
	GetWorld()->RemoveOnActorSpawnedHandler(ActorSpawnedDelegateHandle);
	ActorSpawnedDelegateHandle.Reset();
	
	SkillDuration = WorldDecelerationRate = PlayerAccelerationRate;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_Skill_BulletTime::TraceBulletTimeEffectedActors()
{
	TArray<FHitResult> HitResults;
	const FCollisionQueryParams Params;
	const FVector SweepLocation = GetCurrentActorInfo()->AvatarActor->GetActorLocation();

	// 귀찮아서 매직넘버 처리.
	GetWorld()->SweepMultiByProfile(
		HitResults,
		SweepLocation,
		SweepLocation,
		FQuat::Identity,
		CP_BULLET_TIME_INFLUENCED,
		FCollisionShape::MakeBox(FVector(10000.f, 10000.f, 1000.f)),
		Params
		);

	if (HitResults.IsEmpty())
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckPlayerSkillDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("불릿타임 스킬에 어떤 액터도 영향받지 않음")));
			}
		}
#endif
		
		return;
	}

	TArray<AActor*> HitActors;
	for (auto HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		BulletTimeEffectedActors.Emplace(HitActor);
		HitActors.Emplace(HitActor);
	}
	CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance())->SetActorsCustomTimeDilation(HitActors, WorldDecelerationRate);
}

void ULLL_PGA_Skill_BulletTime::BulletTimeEndedCallBack()
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	if (GetWorld()->GetTimerManager().IsTimerActive(AbilityDurationTimerHandle))
	{
		AbilityDurationTimerHandle.Invalidate();
	}
	BulletTimeActivateSequenceActor->GetSequencePlayer()->GoToEndAndStop();
	
	if (IsValid(BulletTimeDeActivateSequence))
	{
		if (!IsValid(BulletTimeDeActivateSequenceActor))
		{
			BulletTimeDeActivateSequenceActor = GetWorld()->SpawnActorDeferred<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FTransform::Identity);
			FMovieSceneSequencePlaybackSettings Settings;
			Settings.bAutoPlay = false;
			Settings.bHideHud = false;

			if (IsValid(BulletTimeDeActivateSequenceActor))
			{
				BulletTimeDeActivateSequenceActor->PlaybackSettings = Settings;
				BulletTimeDeActivateSequenceActor->SetSequence(BulletTimeDeActivateSequence);
				BulletTimeDeActivateSequenceActor->InitializePlayer();
				BulletTimeDeActivateSequenceActor->FinishSpawning(FTransform::Identity);
			}
		}

		if (IsValid(BulletTimeDeActivateSequenceActor))
		{
			if (IsValid(BulletTimeDeActivateSequenceActor->GetSequencePlayer()))
			{
				BulletTimeDeActivateSequenceActor->GetSequencePlayer()->Play();
			}
		}
	}

	if (BulletTimeEffectedActors.IsEmpty())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	FGameplayAbilityTargetData_ActorArray* ActorArray = new FGameplayAbilityTargetData_ActorArray();
	ActorArray->SetActors(BulletTimeEffectedActors);
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	TargetDataHandle.Add(ActorArray);

	if (ULLL_BaseASC* BaseAsc = CastChecked<ULLL_BaseASC>(GetAbilitySystemComponentFromActorInfo_Checked()); BaseAsc->TryActivateAbilitiesByTag(FGameplayTagContainer(KnockBackCollideCheckTag)))
	{
		BaseAsc->ReceiveTargetData(this, TargetDataHandle);
	}

	TArray<AActor*> EffectedActors;
	for (auto Actor : BulletTimeEffectedActors)
	{
		if (Actor.IsValid())
		{
			EffectedActors.Emplace(Actor.Get());
			if (ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(Actor.Get()))
			{
				KnockBackActor->ApplyStackedKnockBack();
			}
		}
	}
	CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance())->SetActorsCustomTimeDilation(EffectedActors, 1.0f);
	
	BulletTimeEffectedActors.Empty();
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_Skill_BulletTime::OnBulletTimeEffectedActorSpawnCheck(AActor* Actor)
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	const ECollisionResponse Response = Actor->GetComponentsCollisionResponseToChannel(ECC_ENTITY_CHECK);
	if (Response != ECR_Ignore)
	{
		Actor->CustomTimeDilation = WorldDecelerationRate;
		BulletTimeEffectedActors.Emplace(Actor);
	}
}
