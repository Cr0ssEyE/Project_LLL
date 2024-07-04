// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_NiagaraInfoStruct.generated.h"

USTRUCT(BlueprintType)
struct FNiagaraInfo
{
	GENERATED_BODY()

	FNiagaraInfo() :
	Location(FVector::ZeroVector),
	Rotation(FRotator::ZeroRotator),
	Scale(FVector::OneVector),
	UseAbsoluteScale(false),
	AttacheToTarget(true)
	{
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Scale;

	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "AnimNotify")
	uint8 UseAbsoluteScale : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 AttacheToTarget : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};