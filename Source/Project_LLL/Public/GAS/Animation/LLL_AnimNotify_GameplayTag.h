// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueInterface.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "LLL_AnimNotify_GameplayTag.generated.h"

enum class EFModParameter : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AnimNotify_GameplayTag : public UAnimNotify
{
	GENERATED_BODY()

public:
	ULLL_AnimNotify_GameplayTag();
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual void Notify_TagEventTriggered(AActor* OwnerActor);
	virtual void Notify_TagAddOrRemove(AActor* OwnerActor);
	virtual void Notify_CueTriggered(AActor* OwnerActor);
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "이벤트 트리거 사용")
	uint8 bIsUsingTagTrigger : 1;
	
	UPROPERTY(EditAnywhere, DisplayName = "이벤트 트리거", meta=(EditCondition = "bIsUsingTagTrigger == true", EditConditionHides))
	FGameplayTagContainer TriggerGameplayTag;
	
	UPROPERTY(EditAnywhere , DisplayName = "이벤트 레벨", meta=(EditCondition = "bIsUsingTagTrigger == true", EditConditionHides))
	float NotifyLevel;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "태그 부착 사용")
	uint8 bIsUsingGiveTag : 1;
	
	UPROPERTY(EditAnywhere, DisplayName = "태그 부착", meta=(EditCondition = "bIsUsingGiveTag == true", EditConditionHides))
	FGameplayTagContainer OwnerGiveTag;

	UPROPERTY(EditAnywhere , DisplayName = "태그 부착 갯수", meta=(EditCondition = "bIsUsingGiveTag == true", EditConditionHides))
	float GiveTagCount;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "태그 제거 사용")
	uint8 bIsUsingRemoveTag : 1;
	
	UPROPERTY(EditAnywhere, DisplayName = "태그 제거", meta=(EditCondition = "bIsUsingRemoveTag == true", EditConditionHides))
	FGameplayTagContainer OwnerRemoveTag;

	UPROPERTY(EditAnywhere , DisplayName = "태그 제거 갯수", meta=(EditCondition = "bIsUsingRemoveTag == true", EditConditionHides))
	float RemoveTagCount;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "게임플레이 큐 사용")
	uint8 bIsUsingGameplayCue : 1;
	
	UPROPERTY(EditAnywhere, DisplayName = "게임플레이 큐 태그", meta=(Categories = "GameplayCue", EditCondition = "bIsUsingGameplayCue == true", EditConditionHides))
	FGameplayTag GameplayCueTag;

	UPROPERTY(EditAnywhere, DisplayName = "FMOD 파라미터", meta=(EditCondition = "bIsUsingGameplayCue == true", EditConditionHides))
	EFModParameter FModParameter;

	UPROPERTY(EditAnywhere, DisplayName = "FMOD 파라미터 값", meta=(EditCondition = "bIsUsingGameplayCue == true", EditConditionHides))
	float FModParameterValue;
};
