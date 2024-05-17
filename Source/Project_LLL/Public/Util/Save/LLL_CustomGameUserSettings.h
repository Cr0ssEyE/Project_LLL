// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "LLL_CustomGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECT_LLL_API ULLL_CustomGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static ULLL_CustomGameUserSettings* GetCustomGameUserSettings();

	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;
	
	void ApplySoundSettings();

	void ApplyGameplaySettings();

	virtual void SetToDefaults() override;
	
	// 사운드
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMasterSoundVolume() const { return MasterSoundVolume; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBGMSoundVolume() const { return BGMSoundVolume; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetSFXSoundVolume() const { return SFXSoundVolume; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetMasterSoundVolume(float Value) { MasterSoundVolume = Value; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetBGMSoundVolume(float Value) { BGMSoundVolume = Value; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSFXSoundVolume(float Value) { SFXSoundVolume = Value; }

	// 그래픽
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetDisplayResolutionString() const { return DisplayResolution; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SaveScreenResolutionString(FString& Type) { DisplayResolution = Type; }

	// 게임플레이
public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckUsingCameraShake() const { return bUseCameraShake; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCameraShake(bool Value) { bUseCameraShake = Value; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool CheckUsingTargetingCorrection() const { return bUseTargetingCorrection; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetTargetingCorrection(bool Value) { bUseTargetingCorrection = Value; } 

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetGameLanguage() const { return GameLanguageName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetGameLanguage(FString NewLanguage) { GameLanguageName = NewLanguage; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetSubtitleLanguage() const { return SubtitleLanguageName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSubtitleLanguage(FString NewLanguage) { SubtitleLanguageName = NewLanguage; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE uint8 GetSubtitleSize() const { return SubtitleSize; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSubtitleSize(uint8 Size) { SubtitleSize = Size; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE uint8 GetSubtitleSpeed() const { return SubtitleSpeed; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSubtitleSpeed(uint8 Speed) { SubtitleSpeed = Speed; }
	
protected:
	UPROPERTY(Config, SaveGame)
	float MasterSoundVolume;

	UPROPERTY(Config, SaveGame)
	float BGMSoundVolume;

	UPROPERTY(Config, SaveGame)
	float SFXSoundVolume;

protected:
	UPROPERTY(Config, SaveGame)
	FString DisplayResolution;

protected:
	UPROPERTY(Config, SaveGame)
	uint8 bUseCameraShake : 1;
	
	UPROPERTY(Config, SaveGame)
	uint8 bUseTargetingCorrection : 1;

	UPROPERTY(Config, SaveGame)
	FString GameLanguageName;

	UPROPERTY(Config, SaveGame)
	FString SubtitleLanguageName;

	UPROPERTY(Config, SaveGame)
	uint8 SubtitleSize;
	
	UPROPERTY(Config, SaveGame)
	uint8 SubtitleSpeed;
};
