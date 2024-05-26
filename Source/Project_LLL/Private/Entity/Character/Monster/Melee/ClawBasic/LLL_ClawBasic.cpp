// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/ClawBasic/LLL_ClawBasic.h"

#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_Monster_Id.h"
#include "Entity/Character/Monster/Melee/ClawBasic/LLL_ClawBasicAIController.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ClawBasic::ALLL_ClawBasic()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ClawBasicDataAsset>(PATH_CLAW_BASIC_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_ClawBasicAIController::StaticClass();

	Id = ID_CLAW_BASIC;

	LeftClawMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftShoulderGuard"));
	LeftClawMeshComponent->SetCollisionProfileName(CP_NO_COLLISION);
	LeftClawMeshComponent->SetupAttachment(RootComponent);

	RightClawMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightShoulderGuard"));
	RightClawMeshComponent->SetCollisionProfileName(CP_NO_COLLISION);
	RightClawMeshComponent->SetupAttachment(RootComponent);
}

void ALLL_ClawBasic::BeginPlay()
{
	Super::BeginPlay();

	ClawBasicDataAsset = Cast<ULLL_ClawBasicDataAsset>(MeleeMonsterDataAsset);

	LeftClawMeshComponent->SetStaticMesh(ClawBasicDataAsset->ClawMesh);
	LeftClawMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ClawBasicDataAsset->LeftClawAttachSocketName);
	LeftClawMeshComponent->SetRelativeTransform(ClawBasicDataAsset->LeftClawTransform);

	RightClawMeshComponent->SetStaticMesh(ClawBasicDataAsset->ClawMesh);
	RightClawMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ClawBasicDataAsset->RightClawAttachSocketName);
	RightClawMeshComponent->SetRelativeTransform(ClawBasicDataAsset->RightClawTransform);
}
