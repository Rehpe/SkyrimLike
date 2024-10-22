// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SLUserWidget.h"
#include "Item/SLItemDataStructs.h"
#include "SLInventoryWidget.generated.h"


UCLASS()
class SKYRIMLIKE_API USLInventoryWidget : public USLUserWidget
{
	GENERATED_BODY()

public:
	USLInventoryWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	virtual void InitWidget();
	
	UFUNCTION()
	void HandleFilterClicked(bool bIsOn, int inFilterFlag);

	FORCEINLINE void SetInventoryComponent(TObjectPtr<class USLInventoryComponent> InInventoryComponent) { InventoryComponent = InInventoryComponent;};
	
public:
	// Inventory
	UFUNCTION(BlueprintCallable)
	virtual void RefreshInventory();
	
	// Filter
	bool FilterItem(FItemSlot* ItemSlot);
	bool FilterEmpty(FItemSlot* ItemSlot);

protected:
	// Hotbar Widget
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class USLHotbarWidget> HotbarWidget;
	
	// Inventory Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLInventoryComponent> InventoryComponent;

	// Inventory Slot Widget Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLInventorySlotWidget> InventorySlotWidgetClass;
	
	// Widget Binding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UWrapBox> WrapBox_Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USLItemFilterWidget> WB_ItemFilter_Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USLItemFilterWidget> WB_ItemFilter_Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USLItemFilterWidget> WB_ItemFilter_Consumable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USLItemFilterWidget> WB_ItemFilter_Misc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<class USLItemFilterWidget>> FilterList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FilterFlag = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsFilterOn = 0;
};
