// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/SLAvailableEquipmentInventoryWidget.h"
#include "SLAvailableEquipmentSlotWidget.h"
#include "SLEquipmentSlotWidget.h"
#include "SLEquipmentWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Character/SLCharacterPlayer.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WrapBox.h"
#include "GameData/SLGameDataManager.h"
#include "Item/SLItemDataStructs.h"
#include "Global/SLGlobalLibrary.h"
#include "Inventory/SLInventoryComponent.h"

USLAvailableEquipmentInventoryWidget::USLAvailableEquipmentInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLAvailableEquipmentInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AvailableEquipmentSlotWidgetClass)
	{
		AvailableEquipmentSlotWidgetClass = LoadClass<USLAvailableEquipmentSlotWidget>(
			nullptr, TEXT("/Game/SkyrimLike/EquipmentSystem/UI/WB_AvaliableEquipmentSlot.WB_AvaliableEquipmentSlot_C"));
	}

	SetPositionToMouse();
	CreateAvailableEquipmentSlot();
}

void USLAvailableEquipmentInventoryWidget::SetPositionToMouse()
{
	if (Border_Frame)
	{
		// Viewport 상에서의 마우스 위치 가져오기
		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

		if (MousePosition != FVector2D::ZeroVector)
		{
			// Border Frame의 슬롯을 CanvasSlot으로 캐스팅
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Frame->Slot))
			{
				// 마우스 위치로 Border Frame의 위치를 설정
				CanvasSlot->SetPosition(MousePosition);
			}
		}
	}
}

void USLAvailableEquipmentInventoryWidget::HandleOnItemSelected(FName inItemID)
{
	// EquipmentWidget에서 ActiveSlot 불러옴
	if(EquipmentWidget)
	{
		// ItemSlot의 ItemID를 inItemID로 변경 후 새로고침
		EquipmentWidget->GetActiveSlot()->SetItemID(inItemID);
		EquipmentWidget->GetActiveSlot()->RefreshEquipmentSlot();
	}
}

void USLAvailableEquipmentInventoryWidget::CreateAvailableEquipmentSlot()
{
	// 초기화
	WrapBox_Inventory->ClearChildren();
	EquipmentSlots.Empty();

	GetNoneSlot();
	GetFilteredSlot();

	for(FItemSlot ItemSlot : EquipmentSlots)
	{
		AvailableEquipmentSlotWidget = CreateWidget<USLAvailableEquipmentSlotWidget>(GetWorld(), AvailableEquipmentSlotWidgetClass);
		if(AvailableEquipmentSlotWidget)
		{
			AvailableEquipmentSlotWidget->InitiailizeAvailableEquipmentSlot(ItemSlot.ItemID);
			WrapBox_Inventory->AddChild(AvailableEquipmentSlotWidget);
			AvailableEquipmentSlotWidget->OnItemSelected.AddDynamic(this, &USLAvailableEquipmentInventoryWidget::HandleOnItemSelected);
		}
	}
}

void USLAvailableEquipmentInventoryWidget::GetNoneSlot()
{
	const UDataTable* ItemDataTable = USLGameDataManager::Get().GetDataTable();
	if (ItemDataTable)
	{
		TArray<FName> ItemIDs = ItemDataTable->GetRowNames();
		for(FName ItemID : ItemIDs)
		{
			FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
			if (ItemData)
			{
				// 같은 카테고리 & None 이미지
				if((ItemData->EquipmentType == EquipmentCategory) &&
					(ItemData->VisualAssetData.Icon == NoneImage))
				{
					FItemSlot NoneSlot;
					NoneSlot.ItemID = ItemID;
					NoneSlot.ItemQuantity = 1;
					EquipmentSlots.AddUnique(NoneSlot);
				}
			}
		}
	}
}

void USLAvailableEquipmentInventoryWidget::GetFilteredSlot()
{
	ASLCharacterPlayer* Player = USLGlobalLibrary::GetSLPlayerCharacter(GetWorld());
	if (Player)
	{
		// 플레이어 인벤토리 슬롯을 가져와 같은 카테고리의 아이템이 있다면 리스트에 넣기
		USLInventoryComponent* InventoryComponent = Cast<USLInventoryComponent>(Player->GetComponentByClass(USLInventoryComponent::StaticClass()));
		if (InventoryComponent)
		{
			TArray<FItemSlot> ItemSlots =  InventoryComponent->GetItemSlots();
			for(FItemSlot ItemSlot : ItemSlots)
			{
				FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemSlot.ItemID);
				if(ItemData)
				{
					if(ItemData->EquipmentType == EquipmentCategory)
					{
						EquipmentSlots.AddUnique(ItemSlot);
					}
				}
			}
		}
	}
}


