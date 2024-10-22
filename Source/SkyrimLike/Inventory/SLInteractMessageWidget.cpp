// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInteractMessageWidget.h"

#include "Components/TextBlock.h"

USLInteractMessageWidget::USLInteractMessageWidget(const FObjectInitializer& ObjectInitializer)
	: Super (ObjectInitializer)
{
}

void USLInteractMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetInteractMessage(InteractMessage);
}

void USLInteractMessageWidget::SetInteractMessage(const FText& Message)
{
	if(Txt_InteractMessage)
	{
		Txt_InteractMessage->SetText(Message);
	}

	// InteractMessage가 비어있다면 위젯 가시성 Off
	if(Message.IsEmpty())
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
