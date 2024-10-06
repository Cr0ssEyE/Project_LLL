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

	FORCEINLINE FGameplayTagContainer GetTriggerGameplayTag() const { return TriggerGameplayTag; }
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual FString GetNotifyName_Implementation() const override;
	
	virtual void Notify_TagEventTriggered(AActor* OwnerActor);
	virtual void Notify_TagAddOrRemove(AActor* OwnerActor);

protected:
	UPROPERTY(EditAnywhere, DisplayName = "노티파이 이름", meta=(DisplayPriority=1))
	FString NotifyName;
	
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
};
