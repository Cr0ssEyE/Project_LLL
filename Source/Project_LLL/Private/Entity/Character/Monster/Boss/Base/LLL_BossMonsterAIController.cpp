// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/Base/LLL_BossMonsterAIController.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Kismet/GameplayStatics.h"

void ALLL_BossMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	SetPlayer(Player);
}
