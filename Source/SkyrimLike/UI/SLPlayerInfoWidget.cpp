// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLPlayerInfoWidget.h"

#include "CharacterStat/SLCharacterStatComponent.h"
#include "CharacterStat/SLStatusWidget.h"
#include "Equipment/SLEquipmentComponent.h"
#include "Equipment/SLEquipmentWidget.h"
#include "Global/SLGlobalLibrary.h"
#include "Inventory/SLInventoryComponent.h"
#include "Inventory/SLInventoryWidget.h"
#include "Kismet/GameplayStatics.h"

USLPlayerInfoWidget::USLPlayerInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 핫바 리프레시
}

void USLPlayerInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void USLPlayerInfoWidget::CreateWidgets()
{
	CreateInventoryWidget();
	CreateEquipmentWidget();
	CreateStatusWidget();
}

void USLPlayerInfoWidget::OpenWidgets()
{
	ASLCharacterPlayer*  Player = USLGlobalLibrary::GetSLPlayerCharacter(GetWorld());
	EquipmentComponent->CreateWardrobe(Player);
}

void USLPlayerInfoWidget::CloseWidgets()
{
	EquipmentComponent->DestroyWardrobe();
}

void USLPlayerInfoWidget::CreateInventoryWidget()
{
	if(InventoryComponent)
	{
		WB_InventoryWidget = Cast<USLInventoryWidget>(GetWidgetFromName(TEXT("WB_InventoryWidget")));
		WB_InventoryWidget->SetInventoryComponent(InventoryComponent);
		InventoryComponent->SetInventoryWidget(WB_InventoryWidget);
		WB_InventoryWidget->InitWidget();
	}
}

void USLPlayerInfoWidget::CreateEquipmentWidget()
{
	if(EquipmentComponent)
	{
		WB_EquipmentWidget = Cast<USLEquipmentWidget>(GetWidgetFromName(TEXT("WB_EquipmentWidget")));
		WB_EquipmentWidget->SetEquipmentComponent(EquipmentComponent);
		EquipmentComponent->SetEquipmentWidget(WB_EquipmentWidget);
		WB_EquipmentWidget->InitWidget();
	}
}

void USLPlayerInfoWidget::CreateStatusWidget()
{
	if(StatComponent)
	{
		WB_StatusWidget = Cast<USLStatusWidget>(GetWidgetFromName(TEXT("WB_StatusWidget")));
		WB_StatusWidget->SetStatComponent(StatComponent);
		StatComponent->SetStatusWidget(WB_StatusWidget);
		WB_StatusWidget->InitWidget();
	}
}

void USLPlayerInfoWidget::DestroyInventoryWidget()
{
}

void USLPlayerInfoWidget::DestroyEquipmentWidget()
{
}

void USLPlayerInfoWidget::DestroyStatusWidget()
{
}


