// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "SLCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API ASLCharacterNonPlayer : public ASLCharacterBase
{
	GENERATED_BODY()
	
public:
	ASLCharacterNonPlayer();

protected:
	void SetDead() override;
};
