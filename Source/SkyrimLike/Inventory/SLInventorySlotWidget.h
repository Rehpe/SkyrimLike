// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLInventorySlotWidget.generated.h"

// 마우스 / 드래그드롭 Blueprint에서 작성

UCLASS()
class SKYRIMLIKE_API USLInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USLInventorySlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void InitializeSlot(int32 inIndex, FName inItemID, int32 inItemQuantity, TObjectPtr<class USLInventoryComponent> inInventoryComponent)
	 { Index = inIndex; ItemID = inItemID; ItemQuantity = inItemQuantity; InventoryComponent = inInventoryComponent; };
	
protected:
	// Inventory Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLInventoryComponent> InventoryComponent;
	
	// Slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity;
	
	// Widget 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USizeBox> Box_ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_InventorySlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Img_ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_ItemQuantity;

	// Item Prompt
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLItemPromptWidget> ItemPromptWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLItemPromptWidget> ItemPromptWidget;

	// Item Preview
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLItemPreviewSlotWidget> ItemPreviewSlotWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLItemPreviewSlotWidget> ItemPreviewSlotWidget;
	
	// Widget Setting
	void SetNonVisible();
	void SetVisible();
	void SetQuantityText();
	
	// Mouse Event
	UFUNCTION(BlueprintCallable)
	FEventReply HandleMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION(BlueprintCallable)
	UDragDropOperation* HandleOnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& PointerEvent);

	UFUNCTION(BlueprintCallable)
	bool HandleOnDrop(const FGeometry InGeometry, const FPointerEvent& PointerEvent, UDragDropOperation* InOperation);

	UFUNCTION(BlueprintCallable)
	void HandleOnDropCancelled(const FPointerEvent& PointerEvent, UDragDropOperation* Operation);
};
