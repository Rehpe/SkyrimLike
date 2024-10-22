// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLAvailableEquipmentSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, FName, ItemID);

UCLASS()
class SKYRIMLIKE_API USLAvailableEquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USLAvailableEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void InitiailizeAvailableEquipmentSlot(FName inItemID) { ItemID = inItemID; }

public:
	FOnItemSelected OnItemSelected;

	UFUNCTION()
	void HandleOnItemSelected();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_AvailableEquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Img_AvailableEquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
};
