// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Monster/LLL_DamageWidget.h"

void ULLL_DamageWidget::IsAnimationEnd()
{
	AnimationEndDelegate.Broadcast();
}
