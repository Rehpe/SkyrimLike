// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "SLInventoryDDO.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLInventoryDDO : public UDragDropOperation
{
	GENERATED_BODY()

public:
	USLInventoryDDO();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<class USLInventoryComponent> InventoryComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Index;
};
