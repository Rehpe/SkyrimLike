// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/SLItemDataStructs.h"
#include "SLAvailableEquipmentInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLAvailableEquipmentInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	 USLAvailableEquipmentInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void InitializeEquipmentInventoryWidget(EEquipmentCategory inEquipmentCategory, TObjectPtr<class USLEquipmentWidget> inEquipmentWidget)
	{ EquipmentCategory = inEquipmentCategory; EquipmentWidget = inEquipmentWidget; }	

protected:
	UFUNCTION()
	void CreateAvailableEquipmentSlot();

	UFUNCTION()
	void GetNoneSlot();
	
	UFUNCTION()
	void GetFilteredSlot();

	UFUNCTION()
	void SetPositionToMouse();

	UFUNCTION()
	void HandleOnItemSelected(FName ItemID);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EEquipmentCategory EquipmentCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLEquipmentWidget> EquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FItemSlot> EquipmentSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USLAvailableEquipmentSlotWidget> AvailableEquipmentSlotWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLAvailableEquipmentSlotWidget> AvailableEquipmentSlotWidget;
	
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> Border_Frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UWrapBox> WrapBox_Inventory;

	// None Image
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> NoneImage;

	
};
