// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SLPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API ASLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASLPlayerController();

protected:
	virtual void BeginPlay() override;
	
};
