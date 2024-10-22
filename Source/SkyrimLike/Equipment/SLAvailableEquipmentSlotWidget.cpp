// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/SLAvailableEquipmentSlotWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "GameData/SLGameDataManager.h"

USLAvailableEquipmentSlotWidget::USLAvailableEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void USLAvailableEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
	if (ItemData)
	{
		Img_AvailableEquipmentSlot->SetBrushFromTexture(ItemData->VisualAssetData.Icon);
	}

	Button_AvailableEquipmentSlot->OnClicked.AddDynamic(this, &USLAvailableEquipmentSlotWidget::HandleOnItemSelected);
}

void USLAvailableEquipmentSlotWidget::HandleOnItemSelected()
{
	OnItemSelected.Broadcast(ItemID);
}
