// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LLL_BaseCharacter.generated.h"

UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALLL_BaseCharacter();

	FORCEINLINE float GetAttackDistance() const { return AttackDistance; }
	FORCEINLINE float GetTurnSpeed() const { return TurnSpeed; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 캐릭터 상태 설정
public:
	virtual void Attack() {}
	virtual void Dead();

	// 상태 체크용 변수
public:
	FORCEINLINE bool CheckCharacterIsDead() const { return bIsDead; }
	FORCEINLINE uint32 CheckCharacterHealth() const { return Health; }
	
	// 캐릭터 공용 변수
protected:
	UPROPERTY(VisibleAnywhere)
	uint32 Health;

	UPROPERTY(VisibleAnywhere)
	uint32 ShieldAmount;

	UPROPERTY(VisibleAnywhere)
	uint32 OffensePower;
	
	UPROPERTY(VisibleAnywhere)
	float AttackDistance;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsDead : 1;

	// 이동 관련 변수
protected:
	UPROPERTY()
	float MoveSpeed;

	UPROPERTY()
	float AccelerateSpeed;

	UPROPERTY()
	float GroundFriction;

	UPROPERTY()
	float TurnSpeed;

	UPROPERTY()
	FVector MoveDirection;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	// 디버그용 함수
public:
	FORCEINLINE void SetCharacterDead() { bIsDead = true; }
	
	// 디버그용 변수
public:

#endif
};
