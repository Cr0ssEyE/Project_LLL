// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacter.h"

// Sets default values
ALLL_BaseCharacter::ALLL_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsDead = false;
}

// Called when the game starts or when spawned
void ALLL_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALLL_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALLL_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALLL_BaseCharacter::Dead()
{
	if (bIsDead)
	{
		return;
	}

	// Todo: 사망 애니메이션 재생
	
	CurrentHealthAmount = 0;
	bIsDead = true;
}
