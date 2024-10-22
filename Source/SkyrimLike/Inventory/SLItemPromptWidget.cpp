// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLItemPromptWidget.h"

#include "SLInventoryComponent.h"
#include "SLItemSliderWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Global/SLGlobalLibrary.h"

USLItemPromptWidget::USLItemPromptWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void USLItemPromptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯 이벤트 바인딩
	Button_Use->OnClicked.AddDynamic(this, &USLItemPromptWidget::Button_UseClicked);
	Button_Drop->OnClicked.AddDynamic(this, &USLItemPromptWidget::Button_DropClicked);
	Button_Split->OnClicked.AddDynamic(this, &USLItemPromptWidget::Button_SplitClicked);
	Button_Inspect->OnClicked.AddDynamic(this, &USLItemPromptWidget::Button_InspectClicked);

	if (!ItemSliderWidgetClass)
	{
		ItemSliderWidgetClass = LoadClass<USLItemSliderWidget>(
			nullptr, TEXT("/Game/SkyrimLike/InventorySystem/UI/WB_ItemSlider.WB_ItemSlider_C"));
	}
	
	// 마우스 위치에 위젯 생성
	SetWidgetPositionToMouse();
	
	// ItemType이 사용가능한지? 사용불가일 경우 use버튼 비활성화
	// Item Use Message에 따라 Txt_Use 변경
	
}

void USLItemPromptWidget::SetWidgetPositionToMouse()
{
	// Viewport에서 마우스 위치를 가져옴
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

	// CanvasPanelSlot을 얻음
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_ItemPrompt->Slot);
	if(CanvasSlot)
	{
		CanvasSlot->SetPosition(MousePosition);  // 마우스 위치에 위젯 배치
	}
}

void USLItemPromptWidget::Button_UseClicked()
{
	// ItemType이 사용가능한지? 사용불가일 경우 use버튼 비활성화
	InventoryComponent->UseItem(Index);
	RemoveFromParent();
	//FocusInventory
}

void USLItemPromptWidget::Button_DropClicked()
{
	ASLPlayerController* PlayerController = USLGlobalLibrary::GetSLPlayerController(GetWorld());
	ItemSlider = CreateWidget<USLItemSliderWidget>(PlayerController,ItemSliderWidgetClass);
	if(ItemSlider)
	{
		ItemSlider->InitializeSlider(Index, 1, InventoryComponent);
		ItemSlider->AddToViewport();
	}
}

void USLItemPromptWidget::Button_SplitClicked()
{
	InventoryComponent->SplitStack(Index);
	RemoveFromParent();
	//FocusInventory
}

void USLItemPromptWidget::Button_InspectClicked()
{
	int a = 1;
}
