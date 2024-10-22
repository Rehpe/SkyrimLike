// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLItemInfoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLItemInfoInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SKYRIMLIKE_API ISLItemInfoInterface
{
	GENERATED_BODY()

public:
	virtual FName GetItemID() const = 0;
	virtual int32 GetItemQuantity() const = 0;
};
