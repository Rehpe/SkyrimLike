// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SLPlayerController.h"

ASLPlayerController::ASLPlayerController()
{
}

void ASLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 포커스를 뷰포트 안으로 고정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
