// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterAnimInstance.h"
#include "DataAsset/LLL_BaseCharacterDataAsset.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/LLL_EntityInterface.h"
#include "Interface/LLL_FModInterface.h"
#include "LLL_BaseCharacter.generated.h"

class UFMODAudioComponent;
class UWidgetComponent;
class ULLL_BaseCharacterUIManager;
class ULLL_CharacterAttributeSetBase;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDeadDelegate, ALLL_BaseCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOtherActorCollidedDelegate, AActor*, SelfActor, AActor*, OtherActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTakeDamageDelegate, bool, IsDOT);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateWidgetDelegate);

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_BaseCharacter : public ACharacter, public IAbilitySystemInterface, public ILLL_EntityInterface, public ILLL_FModInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALLL_BaseCharacter();

	// 외부 접근용 함수
public:
	FORCEINLINE TObjectPtr<const ULLL_BaseCharacterDataAsset> GetCharacterDataAsset() const { return CharacterDataAsset; }
	FORCEINLINE ULLL_BaseCharacterAnimInstance* GetCharacterAnimInstance() const { return CharacterAnimInstance; }
	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	FORCEINLINE virtual UFMODAudioComponent* GetFModAudioComponent() const override { return FModAudioComponent; }

	// 플레이어
protected:
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void SetDefaultInformation();
	virtual void BeginPlay() override;
	virtual void InitAttributeSet();
	
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
	// 캐릭터 상태 설정
public:
	virtual void Damaged(bool IsDOT = false);
	virtual void Dead();

	// 상태 체크용 변수
public:
	FORCEINLINE bool CheckCharacterIsDead() const { return bIsDead; }

	// 델리게이트
public:
	FCharacterDeadDelegate CharacterDeadDelegate;
	FTakeDamageDelegate TakeDamageDelegate;
	FOtherActorCollidedDelegate OtherActorCollidedDelegate;
	
	// 위젯 업데이트를 위한 델리게이트
public:
	FUpdateWidgetDelegate UpdateWidgetDelegate;
	
	// GAS 변수
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	// 캐릭터 공용 변수
protected:
	UPROPERTY(VisibleAnywhere)
	uint8 bIsDead : 1;

	UPROPERTY(EditAnywhere)
	int32 Level;

	// 이동 관련 변수
protected:
	UPROPERTY(VisibleAnywhere)
	FVector MoveDirection;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BaseCharacterDataAsset> CharacterDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_BaseCharacterAnimInstance> CharacterAnimInstance;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_BaseCharacterUIManager> CharacterUIManager;

protected:
	UPROPERTY(VisibleAnywhere, Category = "FMOD")
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	// 디버그용 함수
public:
	FORCEINLINE void SetCharacterDead() { bIsDead = true; }
	
	// 디버그용 변수
public:
	uint8 bIsSpawned : 1;
#endif
};
