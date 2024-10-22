// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLItemInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLItemInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SKYRIMLIKE_API ISLItemInteractInterface
{
	GENERATED_BODY()

public:
	virtual FText FocusTarget() = 0;

	virtual void Interact(class ASLCharacterPlayer* Player) = 0;

	virtual void OnInteract() = 0;
};
