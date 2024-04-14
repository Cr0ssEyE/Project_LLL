// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/SkillSystem/LLL_PGA_Skill_BulletTime.h"

#include "AbilitySystemComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Game/LLL_GameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

ULLL_PGA_Skill_BulletTime::ULLL_PGA_Skill_BulletTime()
{
	
}

void ULLL_PGA_Skill_BulletTime::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetCurrentActorInfo()->AbilitySystemComponent->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	if (IsValid(PlayerCharacterAttributeSet) && IsValid(BulletTimeActivateSequence))
	{
		SkillDuration = PlayerCharacterAttributeSet->GetBulletTimeDuration();
		WorldDecelerationRate = PlayerCharacterAttributeSet->GetBulletTimeWorldDecelerationRate();
		PlayerAccelerationRate = 1.0f / WorldDecelerationRate;
		GetWorld()->GetWorldSettings()->SetTimeDilation(WorldDecelerationRate);

		if (!IsValid(BulletTimeActivateSequenceActor))
		{
			BulletTimeActivateSequenceActor = GetWorld()->SpawnActorDeferred<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FTransform::Identity);
			FMovieSceneSequencePlaybackSettings Settings;
			Settings.bAutoPlay = false;
			Settings.bHideHud = false;
			Settings.bRestoreState = false;
			BulletTimeActivateSequenceActor->PlaybackSettings = Settings;
			BulletTimeActivateSequenceActor->SetSequence(BulletTimeActivateSequence);
			BulletTimeActivateSequenceActor->FinishSpawning(FTransform::Identity);
		}
		
		ULLL_GameInstance* Instance = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>();
		TArray<TSoftObjectPtr<AActor>> PlayerDependencyActors = Instance->GetPlayerDependencyActors();
		for (auto PlayerActor : PlayerDependencyActors)
		{
			if (!PlayerActor.IsPending())
			{
				PlayerActor.LoadSynchronous()->CustomTimeDilation = PlayerAccelerationRate;
			}
		}
		BulletTimeActivateSequenceActor->CustomTimeDilation = PlayerAccelerationRate;
		BulletTimeActivateSequenceActor->GetSequencePlayer()->SetPlayRate(PlayerAccelerationRate);
		BulletTimeActivateSequenceActor->GetSequencePlayer()->Play();
		
		Instance->PlayerActorAssignedDelegate.AddDynamic(this, &ULLL_PGA_Skill_BulletTime::PlayerActorAssignedCallBack);
		
		GetWorld()->GetTimerManager().SetTimer(AbilityDurationTimerHandle, this, &ULLL_PGA_Skill_BulletTime::BulletTimeEndedCallBack, SkillDuration * WorldDecelerationRate, false);
	}
}

void ULLL_PGA_Skill_BulletTime::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	ULLL_GameInstance* Instance = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>();
	if (IsValid(Instance))
	{
		Instance->PlayerActorAssignedDelegate.RemoveDynamic(this, &ULLL_PGA_Skill_BulletTime::PlayerActorAssignedCallBack);
	}

	if (bWasCancelled)
	{
		BulletTimeEndedCallBack();
	}
	
	TArray<TSoftObjectPtr<AActor>> PlayerDependencyActors = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetPlayerDependencyActors();
	for (auto PlayerActor : PlayerDependencyActors)
	{
		if (!PlayerActor.IsPending())
		{
			PlayerActor.Get()->CustomTimeDilation = 1.0f;
		}
	}

	SkillDuration = WorldDecelerationRate = PlayerAccelerationRate = 0.f;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_Skill_BulletTime::PlayerActorAssignedCallBack(AActor* Actor)
{
	Actor->CustomTimeDilation = PlayerAccelerationRate;
}

void ULLL_PGA_Skill_BulletTime::BulletTimeEndedCallBack()
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	
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
			Settings.bRestoreState = false;

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
			BulletTimeDeActivateSequenceActor->GetSequencePlayer()->Play();
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
