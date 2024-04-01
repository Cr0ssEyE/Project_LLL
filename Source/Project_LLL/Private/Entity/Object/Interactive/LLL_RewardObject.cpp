// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_RewardObject.h"
#include "Util/LLLConstructorHelper.h"

ALLL_RewardObject::ALLL_RewardObject()
{
	RewardMesh = FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'"));
	BaseMesh->SetStaticMesh(RewardMesh);

}
