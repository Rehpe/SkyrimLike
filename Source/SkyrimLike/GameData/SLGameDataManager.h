// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item/SLItemDataStructs.h"
#include "SLGameDataManager.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLGameDataManager : public UObject
{
	GENERATED_BODY()

public:
	USLGameDataManager();
	static USLGameDataManager& Get();

public:
	//UFUNCTION(BlueprintCallable, Category = "ItemData")
	FItemData* GetItemDataByID(FName ItemID) const;
	FORCEINLINE const UDataTable* GetDataTable() const { return ItemDataTable; }; 
	
private:
	UPROPERTY()
	const UDataTable* ItemDataTable ;
};
