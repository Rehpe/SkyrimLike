// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLItemFilterWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

USLItemFilterWidget::USLItemFilterWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLItemFilterWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void USLItemFilterWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USLItemFilterWidget::InitializeItemFilter()
{
	// 이벤트 바인딩
	Button_ItemFilter->OnClicked.AddDynamic(this, &USLItemFilterWidget::OnButtonItemFilterClicked);
	
	SetFilterName();
}

void USLItemFilterWidget::SetFilterName()
{
	if(Txt_FilterName)
	{
		Txt_FilterName->SetText(FilterName);
	}
}

void USLItemFilterWidget::OnButtonItemFilterClicked()
{
	// On/Off값 토글
	bIsOn = !bIsOn;

	// 토글에 따른 색 변경
	FLinearColor Toggled = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("88AFB8FF")));
	FLinearColor UnToggled = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("36464AFF")));
	
	FLinearColor BackgroundColor = bIsOn ? Toggled : UnToggled;
	Button_ItemFilter->SetBackgroundColor(BackgroundColor);

	// 필터 버튼 클릭 이벤트 브로드캐스팅
	OnFilterClicked.Broadcast(static_cast<bool>(bIsOn), static_cast<int>(FilterFlag));
}
