// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"

ULLL_AbnormalStatusAttributeSet::ULLL_AbnormalStatusAttributeSet() :
	MaxMarkStack(10),
	MarkStatusDamageAmplifyPerStack(0.3f),
	MarkStatusDuration(4.f),
	TargetingStatusDamageAmplifyByNormal(2.f),
	TargetingStatusDamageAmplifyByBoss(1.5f),
	TargetingStatusDuration(5.f),
	BleedingStatusPeriod(0.5f),
	BleedingStatusDuration(4.f)
{
	
}
