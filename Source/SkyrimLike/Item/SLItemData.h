// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Interface/SLItemInfoInterface.h"
#include "SLItemData.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKYRIMLIKE_API USLItemData : public UActorComponent, public ISLItemInfoInterface
{
	GENERATED_BODY()

public:	
	USLItemData();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FDataTableRowHandle ItemRowHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32 ItemQuantity = 1;

public:
	// Item Info Interaction
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetItemQuantity() const override { return ItemQuantity; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetItemID() const override { return ItemRowHandle.RowName; }
	
	//ASLItemBase* GetItemPrefab() const override;
	
};
