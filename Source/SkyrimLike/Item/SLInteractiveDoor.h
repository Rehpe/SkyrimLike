// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SLItemEffectInterface.h"
#include "UObject/NoExportTypes.h"
#include "SLInteractiveDoor.generated.h"


UCLASS()
class SKYRIMLIKE_API USLInteractiveDoor : public UObject, public ISLItemEffectInterface
{
	GENERATED_BODY()

public:
	USLInteractiveDoor();

public:
	virtual void Effect();
};
