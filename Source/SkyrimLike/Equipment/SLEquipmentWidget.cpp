// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/SLEquipmentWidget.h"

#include "SLAvailableEquipmentInventoryWidget.h"
#include "SLAvailableEquipmentSlotWidget.h"
#include "SLCharacterWardrobe.h"
#include "SLEquipmentComponent.h"
#include "SLEquipmentSlotWidget.h"
#include "Character/SLCharacterPlayer.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GameData/SLGameDataManager.h"
#include "Interface/SLCharacterWidgetInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Global/SLGlobalLibrary.h"

USLEquipmentWidget::USLEquipmentWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// OwningActor(자신을 소유한 액터)에게 본인을 등록하는 절차가 필요
	// CharacterBase를 직접 참고하게 되면 의존성이 생기며, 확장에 불리함
	// 따라서 위젯을 설정하는 인터페이스를 만들고, CharacterBase가 해당 인터페이스를 상속받게끔 구조 수정
	//ISLCharacterWidgetInterface* CharacterWidget = Cast<ISLCharacterWidgetInterface>(OwningActor);
}

void USLEquipmentWidget::NativeDestruct()
{
	Super::NativeDestruct();

	/*if(AvailableEquipmentInventory)
	{
		AvailableEquipmentInventory->RemoveFromParent();
		AvailableEquipmentInventory = nullptr;
	}
	if(ActiveSlot)
	{
		ActiveSlot->SetActive(false);
		ActiveSlot = nullptr;
	}*/
}

void USLEquipmentWidget::InitWidget()
{
	if (!AvailableEquipmentInventoryClass)
	{
		AvailableEquipmentInventoryClass = LoadClass<USLAvailableEquipmentInventoryWidget>(
			nullptr, TEXT("/Game/SkyrimLike/EquipmentSystem/UI/WB_AvailableEquipmentInventory.WB_AvailableEquipmentInventory_C"));
	}
	
	SetUserNameText();
	InitSlots();
}

void USLEquipmentWidget::RefreshSlots()
{
	for (int i = 0; i < EquipmentSlots.Num(); ++i)
	{
		if (EquipmentSlots[i])
		{
			SetEquipment(static_cast<EEquipmentCategory>(i+1), EquipmentSlots[i],EquipmentSlots[i]->GetItemID());
		}
	}
}

void USLEquipmentWidget::SetUserNameText()
{
	//FText UserName = USLGlobalLibrary::GetUserName(GetWorld());
	FText UserName = FText::FromString("Echo");
	Txt_PlayerName->SetText(UserName);
}

void USLEquipmentWidget::InitSlots()
{
	Slot_Hair = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_Hair")));
	Slot_FacialHair = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_FacialHair")));
	Slot_Accessory = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_Accessory")));
	Slot_Torso = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_Torso")));
	Slot_Hands = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_Hands")));
	Slot_Legs = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_Legs")));
	Slot_MeleeWeapon = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_MeleeWeapon")));
	Slot_RangedWeapon = Cast<USLEquipmentSlotWidget>(GetWidgetFromName(TEXT("Slot_RangedWeapon")));
	
	EquipmentSlots = {Slot_Hair,  Slot_FacialHair, Slot_Accessory,
					Slot_Torso, Slot_Hands, Slot_Legs,
					Slot_MeleeWeapon, Slot_RangedWeapon};
	
	for (int i = 0; i < EquipmentSlots.Num(); ++i)
	{
		if (EquipmentSlots[i])
		{
			EquipmentSlots[i]->InitSlotWidget(EquipmentComponent);
			EquipmentSlots[i]->OnSlotSelected.AddDynamic(this, &USLEquipmentWidget::ToggleSlotActivation);
			SetEquipment(static_cast<EEquipmentCategory>(i+1), EquipmentSlots[i],EquipmentSlots[i]->GetItemID());
		}
	}
}

void USLEquipmentWidget::ToggleSlotActivation(EEquipmentCategory inEquipmentType)
{
	// EquipmentCategory를 int로 변환 -> 배열에 접근(index 값이므로 -1)
	TObjectPtr<class USLEquipmentSlotWidget> SelectedSlot = EquipmentSlots[static_cast<int>(inEquipmentType) - 1];
	
	// 현재 활성화된 슬롯이 있을 경우
	if(ActiveSlot)
	{
		// 선택된 슬롯이 현재 활성화된 슬롯인지 체크
		if(ActiveSlot == SelectedSlot)
		{
			//해당 슬롯 비활성화(토글)
			ActiveSlot->SetActive(false);
			if(AvailableEquipmentInventory)
			{
				AvailableEquipmentInventory->RemoveFromParent();
			}
			ActiveSlot = nullptr;
			return;
		}
		// 이전 활성화된 슬롯 비활성화
		ActiveSlot->SetActive(false);
		if(AvailableEquipmentInventory)
		{
			AvailableEquipmentInventory->RemoveFromParent();
		}
		ActiveSlot = nullptr;
	}

	ActiveSlot = SelectedSlot;
	ActiveSlot->SetActive(true);
	AvailableEquipmentInventory = Cast<USLAvailableEquipmentInventoryWidget>(CreateWidget(GetWorld(), AvailableEquipmentInventoryClass));
	if (AvailableEquipmentInventory)
	{
		AvailableEquipmentInventory->InitializeEquipmentInventoryWidget(inEquipmentType, this);
		AvailableEquipmentInventory->AddToViewport();
	}
}

void USLEquipmentWidget::SetEquipment(EEquipmentCategory inEquipmentCategory,
		TObjectPtr<class USLEquipmentSlotWidget> EquippedSlotWidget,
		FName ItemID)
{
	if(EquipmentComponent)
	{
		FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);

		// 해당 장비 카테고리를 찾아, 장착 중인 아이템을 들어온 ItemID로 변경
		TMap<EEquipmentCategory, FName> EquipList = EquipmentComponent->GetEquipmentList();
		FName* FoundItem = EquipList.Find(inEquipmentCategory);
		if(FoundItem)
		{
			*FoundItem = ItemID;
			EquippedSlotWidget->SetItemID(ItemID);
		}
		// 해당 장비 카테고리를 찾지 못했다면 ItemID를 none으로 변경
		else
		{
			EquippedSlotWidget->SetItemID(FName(TEXT("")));
		}
		EquippedSlotWidget->UpdateSlot();
	}
}
