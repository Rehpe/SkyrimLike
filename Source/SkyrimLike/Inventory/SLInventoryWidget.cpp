// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryWidget.h"

#include "SLHotbarWidget.h"
#include "SLInventoryComponent.h"
#include "SLInventorySlotWidget.h"
#include "SLItemFilterWidget.h"
#include "Components/WrapBox.h"
#include "Global/SLGlobalLibrary.h"
#include "GameData/SLGameDataManager.h"

USLInventoryWidget::USLInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void USLInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USLInventoryWidget::InitWidget()
{
	WB_ItemFilter_Weapon = Cast<USLItemFilterWidget>(GetWidgetFromName(TEXT("WB_ItemFilter_Weapon")));
	WB_ItemFilter_Equipment = Cast<USLItemFilterWidget>(GetWidgetFromName(TEXT("WB_ItemFilter_Equipment")));
	WB_ItemFilter_Consumable = Cast<USLItemFilterWidget>(GetWidgetFromName(TEXT("WB_ItemFilter_Consumable")));
	WB_ItemFilter_Misc = Cast<USLItemFilterWidget>(GetWidgetFromName(TEXT("WB_ItemFilter_Misc")));
	
	FilterList = { WB_ItemFilter_Weapon, WB_ItemFilter_Equipment, WB_ItemFilter_Consumable, WB_ItemFilter_Misc};
	
	for (USLItemFilterWidget* FilterButton : FilterList)
	{
		if(FilterButton)
		{
			FilterButton->OnFilterClicked.AddDynamic(this, &USLInventoryWidget::HandleFilterClicked);
			FilterButton->InitializeItemFilter();
		}
	}
	if(InventoryComponent)
	{
		// 이벤트 바인딩
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &USLInventoryWidget::RefreshInventory);
	}
	if (!InventorySlotWidgetClass)
	{
		InventorySlotWidgetClass = LoadClass<USLInventorySlotWidget>(
			nullptr, TEXT("/Game/SkyrimLike/InventorySystem/UI/WBP_InventorySlot.WBP_InventorySlot_C"));
	}
	if(!HotbarWidget)
	{
		HotbarWidget = Cast<USLHotbarWidget>(GetWidgetFromName(TEXT("WB_Hotbar")));
		//HotbarWidget->SetInventoryComponent(InventoryComponent);
	}
	RefreshInventory();
}

void USLInventoryWidget::RefreshInventory()
{
	if(InventoryComponent)
	{
		WrapBox_Inventory->ClearChildren();
		HotbarWidget->GetHotbarWrapBox()->ClearChildren();
		
		TArray<FItemSlot> ItemSlots = InventoryComponent->GetItemSlots();

		// Hotbar
		for(int i =0; i < InventoryComponent->GetHotbarSize(); i++)
		{
			FItemSlot ItemSlot = ItemSlots[i];
			FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemSlot.ItemID);
			
			ASLPlayerController* PlayerController = USLGlobalLibrary::GetSLPlayerController(GetWorld());
			USLInventorySlotWidget* SlotWidget = CreateWidget<USLInventorySlotWidget>(PlayerController,InventorySlotWidgetClass);

			if(SlotWidget)
			{
				SlotWidget->InitializeSlot(i, ItemSlot.ItemID, ItemSlot.ItemQuantity, InventoryComponent);
				HotbarWidget->GetHotbarWrapBox()->AddChildToWrapBox(SlotWidget);
			}
		}

		// Inventory
		for(int i =0; i < ItemSlots.Num(); i++)
		{
			FItemSlot ItemSlot = ItemSlots[i];
			FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemSlot.ItemID);
			
			// 필터 결과 보여질 슬롯만 인벤토리 슬롯위젯 생성
			if((!bIsFilterOn && FilterEmpty(&ItemSlot) || FilterItem(&ItemSlot)))
			{
				ASLPlayerController* PlayerController = USLGlobalLibrary::GetSLPlayerController(GetWorld());
				USLInventorySlotWidget* SlotWidget = CreateWidget<USLInventorySlotWidget>(PlayerController,InventorySlotWidgetClass);

				if(SlotWidget)
				{
					SlotWidget->InitializeSlot(i, ItemSlot.ItemID, ItemSlot.ItemQuantity, InventoryComponent);
					WrapBox_Inventory->AddChildToWrapBox(SlotWidget);
				}
			}
		}
	}
}

bool USLInventoryWidget::FilterItem(FItemSlot* ItemSlot)
{
	// 필터가 적용되지 않았다면 전부 보여야하므로 true
	if(!bIsFilterOn) return true;
	
	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemSlot->ItemID);
	if(ItemData)
	{
		// 아이템의 타입을 비트마스크로 필터링
		int32 ItemTypeBitMask = static_cast<int32>(ItemData->ItemType); //static_cast<int32>(FMath::TruncToInt(FMath::Pow(2.0f, ItemData->ItemType)));

		return (ItemTypeBitMask & FilterFlag) != 0;
	}
	// ItemData가 없으면 표시하지 않음
	return false;
}

bool USLInventoryWidget::FilterEmpty(FItemSlot* ItemSlot)
{
	return (ItemSlot->ItemQuantity == 0);
}

void USLInventoryWidget::HandleFilterClicked(bool bIsOn, int inFilterFlag)
{
	if(bIsOn)
	{
		// 필터 추가 (OR 연산)
		FilterFlag |= inFilterFlag;
	}
	else
	{
		// 필터 제거 (XOR 연산)
		FilterFlag &= ~inFilterFlag;
	}

	// 선택된 필터가 있는지 확인
	if(FilterFlag == 0)	bIsFilterOn = false;
	else bIsFilterOn = true;

	// 인벤토리 업데이트
	RefreshInventory();
}
