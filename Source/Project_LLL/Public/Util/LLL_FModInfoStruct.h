// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "LLL_FModInfoStruct.generated.h"

class UFMODEvent;

USTRUCT(BlueprintType)
struct FFModInfo
{
	GENERATED_BODY()

	FFModInfo() :
	FModEvent(nullptr),
	FModParameter(EFModParameter::None)
	{
	
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFMODEvent* FModEvent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFModParameter FModParameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};