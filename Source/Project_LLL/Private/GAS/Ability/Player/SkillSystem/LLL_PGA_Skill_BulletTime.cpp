// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/SkillSystem/LLL_PGA_Skill_BulletTime.h"

#include "AbilitySystemComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneCaptureSettings.h"
#include "Game/LLL_GameInstance.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

void ULLL_PGA_Skill_BulletTime::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetCurrentActorInfo()->AbilitySystemComponent->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	if (IsValid(PlayerCharacterAttributeSet))
	{
		SkillDuration = PlayerCharacterAttributeSet->GetBulletTimeDuration();
		WorldDecelerationRate = PlayerCharacterAttributeSet->GetBulletTimeWorldDecelerationRate();
		PlayerAccelerationRate = 1.0f / WorldDecelerationRate;
		GetWorld()->GetWorldSettings()->SetTimeDilation(WorldDecelerationRate);
		
		FMovieSceneSequencePlaybackSettings Settings;
		Settings.bAutoPlay = true;
		Settings.bHideHud = false;
		Settings.bRestoreState = false;
	
		ALevelSequenceActor* LevelSequenceActor = NewObject<ALevelSequenceActor>();
		ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BulletTimeActivateSequence, Settings, LevelSequenceActor);
		LevelSequenceActor->CustomTimeDilation = PlayerAccelerationRate;

		ULLL_GameInstance* Instance = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>();
		TArray<TSoftObjectPtr<AActor>> PlayerDependencyActors = Instance->GetPlayerDependencyActors();
		for (auto PlayerActor : PlayerDependencyActors)
		{
			if (!PlayerActor.IsPending())
			{
				PlayerActor.Get()->CustomTimeDilation = PlayerAccelerationRate;
			}
		}

		Instance->PlayerActorAssignedDelegate.AddDynamic(this, &ULLL_PGA_Skill_BulletTime::PlayerActorAssignedCallBack);
		
		FTimerHandle AbilityDurationTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(AbilityDurationTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]()
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}), SkillDuration, false);
	}
}

void ULLL_PGA_Skill_BulletTime::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = true;
	Settings.bHideHud = false;
	Settings.bRestoreState = false;

	ALevelSequenceActor* Actor = NewObject<ALevelSequenceActor>();
	ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), BulletTimeDeActivateSequence, Settings, Actor);

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
