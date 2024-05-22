// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Interface/LLL_DashMonsterInterface.h"
#include "Entity/Character/Monster/Melee/Base/LLL_MeleeMonster.h"
#include "LLL_SwordDash.generated.h"

class ULLL_SwordDashAttributeSet;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_SwordDash : public ALLL_MeleeMonster, public ILLL_DashMonsterInterface
{
	GENERATED_BODY()

public:
	ALLL_SwordDash();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	FORCEINLINE virtual void SetDashing(bool IsDashing) override { bIsDashing = IsDashing; }
	FORCEINLINE virtual bool IsDashing() const override { return bIsDashing; }
	
	virtual void Dash() const override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_SwordDashDataAsset> SwordDashDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_SwordDashAttributeSet> SwordDashAttributeSet;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DashDamageRangeBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> SwordMeshComponent;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsDashing : 1;
	
	// Todo : 어빌리티 작업이 끝난 후 커브 데이터로 옮기기
	TSubclassOf<UGameplayEffect> InitEffect;
};
