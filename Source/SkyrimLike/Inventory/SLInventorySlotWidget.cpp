// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventorySlotWidget.h"

#include "SLInventoryComponent.h"
#include "SLInventoryDDO.h"
#include "SLItemPreviewSlotWidget.h"
#include "SLItemPromptWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "GameData/SLGameDataManager.h"
#include "SlateCore.h"
#include "Input/Reply.h"


USLInventorySlotWidget::USLInventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLInventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
	if (ItemData)
	{
		UTexture2D* ItemIcon = ItemData->VisualAssetData.Icon;
		Img_ItemIcon->SetBrushFromTexture(ItemIcon);
		SetQuantityText();
		SetVisible();
	}
	else
	{
		SetNonVisible();
	}

	if (!ItemPromptWidgetClass)
	{
		ItemPromptWidgetClass = LoadClass<USLItemPromptWidget>(
			nullptr, TEXT("/Game/SkyrimLike/InventorySystem/UI/WBP_ItemPromt.WBP_ItemPromt_C"));
	}
	if (!ItemPreviewSlotWidgetClass)
	{
		ItemPreviewSlotWidgetClass = LoadClass<USLItemPreviewSlotWidget>(
			nullptr, TEXT("/Game/SkyrimLike/InventorySystem/UI/WB_PreviewSlot.WB_PreviewSlot_C"));
	}
}

void USLInventorySlotWidget::SetNonVisible()
{
	Img_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	Box_ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
}

void USLInventorySlotWidget::SetVisible()
{
	Img_ItemIcon->SetVisibility(ESlateVisibility::Visible);
	Box_ItemQuantity->SetVisibility(ESlateVisibility::Visible);
}

void USLInventorySlotWidget::SetQuantityText()
{
	Txt_ItemQuantity->SetText(FText::AsNumber(ItemQuantity));
}

FEventReply USLInventorySlotWidget::HandleMouseButtonDown(const FGeometry& InGeometry,
                                                          const FPointerEvent& InMouseEvent)
{
	if (ItemID != "")
	{
		// 오른쪽 마우스 버튼 클릭시
		if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
		{
			if (ItemPromptWidget)
			{
				ItemPromptWidget->RemoveFromParent();
			}

			// ItemPrompt 위젯 생성
			ItemPromptWidget = CreateWidget<USLItemPromptWidget>(GetWorld(), ItemPromptWidgetClass);
			if (ItemPromptWidget)
			{
				ItemPromptWidget->SetOwningPlayer(GetOwningPlayer());
				ItemPromptWidget->InitializePrompt(Index, InventoryComponent);
				ItemPromptWidget->AddToViewport();
			}
			FEventReply Reply;
			Reply.NativeReply = FReply::Handled();
			return Reply;
		}

		// 왼쪽 마우스 버튼 클릭시
		if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
		{
			if (ItemPromptWidget)
			{
				ItemPromptWidget->RemoveFromParent();
			}

			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		}
	}
	return FEventReply(false);
}

UDragDropOperation* USLInventorySlotWidget::HandleOnDragDetected(const FGeometry& MyGeometry,
                                                                 const FPointerEvent& PointerEvent)
{
	// ItemPreviewSlot 위젯 생성
	ItemPreviewSlotWidget = CreateWidget<USLItemPreviewSlotWidget>(GetWorld(), ItemPreviewSlotWidgetClass);
	if (ItemPreviewSlotWidget)
	{
		ItemPreviewSlotWidget->SetOwningPlayer(GetOwningPlayer());
		ItemPreviewSlotWidget->SetItemID(ItemID);
	}

	USLInventoryDDO* DDO = NewObject<USLInventoryDDO>(this, USLInventoryDDO::StaticClass());
	if (DDO)
	{
		DDO->DefaultDragVisual = ItemPreviewSlotWidget;
		DDO->Payload = this;
		DDO->Pivot = EDragPivot::CenterCenter;
		DDO->InventoryComponent = InventoryComponent;
		DDO->Index = Index;

		return DDO;
	}
	return nullptr;
}

bool USLInventorySlotWidget::HandleOnDrop(const FGeometry InGeometry, const FPointerEvent& PointerEvent,
	UDragDropOperation* InOperation)
{
	USLInventoryDDO* DraggedSlot = Cast<USLInventoryDDO>(InOperation);
	if (DraggedSlot)
	{
		// 드래그 받을 슬롯과 드래그하는 슬롯의 InventoryComponent가 다르거나, 인덱스 번호가 달라야함
		if(DraggedSlot->InventoryComponent != this->InventoryComponent ||
			DraggedSlot->Index != this->Index)
		{
			InventoryComponent->TransformSlot(Index, DraggedSlot->Index, DraggedSlot->InventoryComponent);
			return true;
		}
	}
	return false;
}

void USLInventorySlotWidget::HandleOnDropCancelled(const FPointerEvent& PointerEvent, UDragDropOperation* Operation)
{
	if(InventoryComponent)
	{
		// 슬롯 인덱스에 해당하는 모든 아이템을 인벤토리에서 제거
		InventoryComponent->RemoveAllFromInventory(Index);
	}
}
