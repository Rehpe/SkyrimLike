// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLItemSliderWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

USLItemSliderWidget::USLItemSliderWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLItemSliderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_ItemSlider->Slot))
	{
		CanvasSlot->SetPosition(MousePosition);
	}

	SetSliderValue();
	SetSliderText();

	// 위젯 이벤트 바인딩
	if(Slider)
		Slider->OnValueChanged.AddDynamic(this, &USLItemSliderWidget::OnSliderValueChanged);
	if (Button_Drop)
		Button_Drop->OnClicked.AddDynamic(this, &USLItemSliderWidget::OnDropButtonClicked);
	if (Button_Cancel)
		Button_Cancel->OnClicked.AddDynamic(this, &USLItemSliderWidget::OnCancelButtonClicked);
}

void USLItemSliderWidget::OnSliderValueChanged(float Value)
{
	ItemQuantity = FMath::TruncToInt(Value);
	Txt_ItemQuantity->SetText(FText::AsNumber(ItemQuantity));
}

void USLItemSliderWidget::OnDropButtonClicked()
{
	// 인벤토리에서 아이템 제거
	//InventoryComponent->RemoveFromInventory(Index, ItemQuantity);

	// UI에서 위젯 제거
	RemoveFromParent();
}

void USLItemSliderWidget::OnCancelButtonClicked()
{
	RemoveFromParent();
}

void USLItemSliderWidget::SetSliderValue()
{
	Slider->SetValue(ItemQuantity);
	Slider->SetMaxValue(ItemQuantity);

	ItemQuantity = Slider->GetValue();
}

void USLItemSliderWidget::SetSliderText()
{
	Txt_ItemQuantity->SetText(FText::AsNumber(ItemQuantity));
}	
