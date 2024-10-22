// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLItemPreviewSlotWidget.h"

#include "Components/Image.h"
#include "GameData/SLGameDataManager.h"

USLItemPreviewSlotWidget::USLItemPreviewSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLItemPreviewSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
	if(ItemData)
	{
		Img_ItemImage->SetBrushFromTexture(ItemData->VisualAssetData.Icon);
	}
}

void USLItemPreviewSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

