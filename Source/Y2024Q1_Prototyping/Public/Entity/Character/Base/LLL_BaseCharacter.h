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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 상태 체크용 변수
public:
	FORCEINLINE bool CheckCharacterIsDead() { return bIsDead; }
	FORCEINLINE uint32 CheckCharacterHealth() { return CharacterHealthAmount; }
	
	// 캐릭터 공용 변수
protected:
	UPROPERTY(VisibleAnywhere)
	uint32 CharacterHealthAmount;

	UPROPERTY(VisibleAnywhere)
	uint32 CharacterShieldAmount;

	UPROPERTY(VisibleAnywhere)
	uint32 CharacterOffensePower;

	UPROPERTY(VisibleAnywhere)
	uint32 CharacterMoveSpeed;

	UPROPERTY(VisibleAnywhere)
	uint32 bIsDead : 1;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	// 디버그용 함수
public:
	FORCEINLINE void SetCharacterDead() { bIsDead = true; }
	
	// 디버그용 변수
public:

#endif
};
