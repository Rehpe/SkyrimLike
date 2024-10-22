// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/SLEquipmentSlotWidget.h"

#include "SLEquipmentComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameData/SLGameDataManager.h"

USLEquipmentSlotWidget::USLEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USLEquipmentSlotWidget::InitSlotWidget(TObjectPtr<class USLEquipmentComponent> inEquipmentComponent)
{
	EquipmentComponent = inEquipmentComponent;
	
	// 슬롯버튼 클릭 바인딩
	if (Button_EquipmentSlot)
	{
		// 슬롯 버튼이 클릭될 때 호출되는 함수 연결
		if(!Button_EquipmentSlot->OnClicked.IsBound())
		{
			Button_EquipmentSlot->OnClicked.AddDynamic(this, &USLEquipmentSlotWidget::OnButtonClicked);
		}
	}
	RefreshEquipmentSlot();
}

void USLEquipmentSlotWidget::UpdateSlot()
{
	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);

	if(ItemData)
	{
		SetSlot(*ItemData);
	}
	else
	{
		SetSlotEmpty();
	}
}

void USLEquipmentSlotWidget::SetActive(bool bIsActive)
{
	if (Border_ActiveSlot)
	{
		//슬롯이 활성화되면 Visible, 비활성화되면 Collapsed
		Border_ActiveSlot->SetVisibility(bIsActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void USLEquipmentSlotWidget::SetSlot(FItemData& ItemData)
{
	UTexture2D* ItemIcon = ItemData.VisualAssetData.Icon;

	// 이부분 수정 필요! 
	// 현재 슬롯의 아이콘이 빈 슬롯 아이콘이라면
	if(ItemData.ID == "None")
	{
		SetSlotEmpty();
		return;
	}
	Img_EquipmentSlot->SetBrushFromTexture(ItemIcon);
	Img_EmptySlot->SetVisibility(ESlateVisibility::Collapsed);
	Img_EquipmentSlot->SetVisibility(ESlateVisibility::Visible);
}

void USLEquipmentSlotWidget::SetSlotEmpty()
{
	// EmptySlot Visible, EquipmentSlot Collapsed
	Img_EmptySlot->SetBrushFromTexture(EmptySlotImage.Get());
	Img_EmptySlot->SetVisibility(ESlateVisibility::Visible);
	Img_EquipmentSlot->SetVisibility(ESlateVisibility::Collapsed);
}

void USLEquipmentSlotWidget::OnButtonClicked()
{
	OnSlotSelected.Broadcast(EquipmentCategory);
}

void USLEquipmentSlotWidget::RefreshEquipmentSlot()
{
	UpdateSlot();
	
	if(EquipmentComponent)
	{
		EquipmentComponent->EquipItem(EquipmentCategory, ItemID);
	}
}


