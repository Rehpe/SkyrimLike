// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/SLInventoryComponent.h"

#include "Item/SLItemDataStructs.h"
#include "SLInteractMessageWidget.h"
#include "SLInventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameData/SLGameDataManager.h"
#include "Global/SLGlobalLibrary.h"
#include "Interface/SLItemInteractInterface.h"
#include "Item/SLItemBase.h"
#include "Physics/SLCollision.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

class ASLItemBase;
// Sets default values for this component's properties
USLInventoryComponent::USLInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USLInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ItemSlots.SetNum(InventorySize);

	if (!InteractMessageWidgetClass)
	{
		InteractMessageWidgetClass = LoadClass<USLInteractMessageWidget>(
			nullptr, TEXT("/Game/SkyrimLike/InventorySystem/UI/WB_InteractMessage.WB_InteractMessage_C"));
	}
	if (!InventoryWidgetClass)
	{
		InventoryWidgetClass = LoadClass<USLInventoryWidget>(
			nullptr, TEXT("/Game/SkyrimLike/InventorySystem/UI/WBP_Inventory.WBP_Inventory_C"));
	}
	CreateInteractMessage();

	ItemSlots[0].ItemID =  FName(TEXT("Hair_01"));
	ItemSlots[0].ItemQuantity =  1;

	ItemSlots[1].ItemID =  FName(TEXT("FacialHair_01"));
	ItemSlots[1].ItemQuantity =  1;
	
	ItemSlots[2].ItemID =  FName(TEXT("Accessory_01"));
	ItemSlots[2].ItemQuantity =  1;
}

TObjectPtr<class USLInventoryWidget> USLInventoryComponent::CreateInventoryWidget()
{
	ASLPlayerController* PlayerController = USLGlobalLibrary::GetSLPlayerController(GetWorld());
	InventoryWidget = CreateWidget<USLInventoryWidget>(PlayerController,InventoryWidgetClass);
	if(InventoryWidget)
	{
		InventoryWidget->SetOwningPlayer(Cast<APlayerController>(PlayerController));
		InventoryWidget->SetInventoryComponent(this);
		return InventoryWidget;
	}
	return nullptr;
}

void USLInventoryComponent::CreateInteractMessage()
{
	InteractMessageWidget = Cast<USLInteractMessageWidget>(CreateWidget(GetWorld(), InteractMessageWidgetClass));
	if(InteractMessageWidget)
	{
		InteractMessageWidget->AddToViewport();
	}
}

void USLInventoryComponent::DetectTargetActor()
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = GetOwner()->GetActorLocation();
	
	// Sphere Trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner()); // 자기 자신은 무시
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		CCHANNEL_SLINTERACTION,  // SLInteractive로 설정된 채널
		FCollisionShape::MakeSphere(InteractRadius),
		QueryParams
	);
	// 감지된 아이템이 있는 경우
	if (bHit && HitResult.GetActor() != nullptr)
	{
		// 기존 타겟이 있었던 경우
		if (TargetActor)
		{
			// 기존 TargetActor와 다른 아이템을 감지한 경우
			if (TargetActor != HitResult.GetActor())
			{
				// 기존 타겟 Custrom Depth Render 비활성화
				UStaticMeshComponent* Mesh = TargetActor->FindComponentByClass<UStaticMeshComponent>();
				if (Mesh)
				{
					Mesh->SetRenderCustomDepth(false);
				}
			}
		}
		// 타겟 재설정
		TargetActor = HitResult.GetActor();

		// 새로운 타겟 Custrom Depth Render 활성화
		UStaticMeshComponent* Mesh = TargetActor->FindComponentByClass<UStaticMeshComponent>();
		if (Mesh)
		{
			Mesh->SetRenderCustomDepth(true);
		}

		// Interact 인터페이스 확인 후 InteractMessage 생성
		ISLItemInteractInterface* Interactable = Cast<ISLItemInteractInterface>(TargetActor);
		if (Interactable)
		{
			FText Message = Interactable->FocusTarget();
			InteractMessageWidget->SetInteractMessage(Message);
		}
	}
	// 감지된 아이템이 없는 경우
	else
	{
		// 기존 타겟 Custrom Depth Render 비활성화
		if(TargetActor)
		{
			UStaticMeshComponent* Mesh = TargetActor->FindComponentByClass<UStaticMeshComponent>();
			if(Mesh)
			{
				Mesh->SetRenderCustomDepth(false);
			}
		}
		// Target 비우기
		TargetActor = nullptr;

		// 빈 메세지 설정
		InteractMessageWidget->SetInteractMessage(FText::GetEmpty());
	}
}

void USLInventoryComponent::BeginDetect()
{
	FTimerHandle TimerHandle;

	if (GetOwner() && GetOwner()->GetWorld())
	{
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&USLInventoryComponent::DetectTargetActor,
			0.2f,
			true // 반복 실행
		);
	}
}

void USLInventoryComponent::TryInteract()
{
	if(!TargetActor) return;

	ISLItemInteractInterface* Interactable = Cast<ISLItemInteractInterface>(TargetActor);
	if(Interactable)
	{
		ASLCharacterPlayer* Player =  USLGlobalLibrary::GetSLPlayerCharacter(GetWorld());
		Interactable->Interact(Player);
	}
}

void USLInventoryComponent::UpdateInventory()
{
	OnInventoryUpdated.Broadcast();
}

int32 USLInventoryComponent::FindExistingItemSlot(FName ItemID)
{
	for(int i = 0; i < ItemSlots.Num(); i++)
	{
		FItemSlot Slot = ItemSlots[i];
		// 슬롯 중 ItemID가 동일하며 최대스택 미만인 슬롯 번호 반환
		if(Slot.ItemID == ItemID && Slot.ItemQuantity < GetMaxStackSize(ItemID))
		{
			return i;
		}
	}
	return -1;
}

int32 USLInventoryComponent::FindEmptySlot()
{
	for(int i = 0; i < ItemSlots.Num(); i++)
	{
		FItemSlot Slot = ItemSlots[i];
		if(Slot.ItemQuantity == 0)
		{
			return i;
		}
	}
	return -1;
}

void USLInventoryComponent::TransformSlot(int32 Index, int32 SourceIndex, USLInventoryComponent* SourceInventory)
{
	FItemSlot& SourceSlot = SourceInventory->GetItemSlots()[SourceIndex];
	FItemSlot& TargetSlot = this->ItemSlots[Index];

	FName SourceID = SourceSlot.ItemID;
	int32 SourceQuantity = SourceSlot.ItemQuantity;
	FName TargetID = TargetSlot.ItemID;
	int32 TargetQuantity = TargetSlot.ItemQuantity;
	
	// Target 칸에도 같은 아이템이 들어있다면 합친다
	if(TargetSlot.ItemID == SourceSlot.ItemID)
	{
		// 두 아이템의 개수를 합치고, MaxStackSize에서 뺀다
		int32 SumQuantity = TargetQuantity + SourceQuantity;
		int32 RemainingQuantity =  SumQuantity - GetMaxStackSize(TargetID);
		int32 ClampedQuantity = FMath::Clamp(SumQuantity, 0, GetMaxStackSize(TargetID));

		// Target칸의 ItemQuantity는 ClampedQuantity로 조정
		TargetSlot.ItemQuantity = ClampedQuantity;

		// Remain이 있다면, 즉 두 아이템의 합이 MaxStack을 초과한다면 Source 슬롯의 아이템 갯수 Remain으로 조정
		if(RemainingQuantity > 0)
		{
			CreateNewStack(ItemSlots[Index].ItemID, RemainingQuantity, SourceIndex);	
		}
		// Remain이 음수라면 두 아이템이 한 슬롯에 합쳐졌다는 의미이므로, SourceSlot을 비운다
		else
		{
			ClearSlot(SourceIndex);
		}
	}
	// Target 칸이 다른 Slot이라면 두 Slot Swap
	else
	{
		SourceInventory->CreateNewStack(TargetID, TargetQuantity, SourceIndex);
		CreateNewStack(SourceID, SourceQuantity, Index);
	}

	// 효과음 재생
	FItemData* Data = USLGameDataManager::Get().GetItemDataByID(SourceID);
	if(Data->SFXData.MoveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Data->SFXData.MoveSound, GetOwner()->GetActorLocation());
	}
	
	// 업데이트 알림
	UpdateInventory();
}

void USLInventoryComponent::ClearSlot(int32 Index)
{
	ItemSlots[Index].ItemID = FName("None");
	ItemSlots[Index].ItemQuantity = 0;
}

void USLInventoryComponent::RemoveFromInventory(int32 Index, int32 RemoveQuantity)
{
	FItemSlot TempSlot = ItemSlots[Index];
	FName TempItemName = TempSlot.ItemID;
	int32 TempQuantity = TempSlot.ItemQuantity;

	// 슬롯 내의 모든 양을 Drop하는 경우 해당 슬롯 비우기
	if(TempQuantity == RemoveQuantity)
	{
		ClearSlot(Index);
	}
	else
	{
		RemoveFromStack(Index, RemoveQuantity);
	}
	
	// ItemDrop
	DropItem(TempItemName, RemoveQuantity);

	// 업데이트 알림
	UpdateInventory();
}

void USLInventoryComponent::RemoveAllFromInventory(int32 Index)
{
	FItemSlot TempSlot = ItemSlots[Index];
	FName TempItemID = TempSlot.ItemID;
	int32 TempQuantity = TempSlot.ItemQuantity;

	ClearSlot(Index);
	DropItem(TempItemID, TempQuantity);

	// 효과음 재생
	FItemData* Data = USLGameDataManager::Get().GetItemDataByID(TempItemID);
	if(Data->SFXData.DropSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Data->SFXData.DropSound, GetOwner()->GetActorLocation());
	}

	// 업데이트 알림
	UpdateInventory();
}

void USLInventoryComponent::CreateNewStack(FName ItemID, int32 ItemQuantity, int32 Index)
{
	FItemSlot& Slot = ItemSlots[Index];
	Slot.ItemID = ItemID;
	Slot.ItemQuantity = ItemQuantity;	
}

void USLInventoryComponent::AddToStack(int32 Index, int32 inItemQuantity)
{
	ItemSlots[Index].ItemQuantity += inItemQuantity; 
}

void USLInventoryComponent::SplitStack(int32 Index)
{
	// 원본 칸에 들어있던 아이템 정보 임시 저장
	FName TempItemID = ItemSlots[Index].ItemID;
	int32 TempItemQuantity = ItemSlots[Index].ItemQuantity;

	// 아이템 수량이 1이면 나눌 수 없음
	if(TempItemQuantity == 1) return;

	// 빈 슬롯이 없으면 나눌 수 없음
	int32 EmptySlotIndex = FindEmptySlot();
	if(EmptySlotIndex == -1) return;

	// 새로운 슬롯에 아이템 절반만큼 생성
	ItemSlots[EmptySlotIndex].ItemID = TempItemID;
	ItemSlots[EmptySlotIndex].ItemQuantity = TempItemQuantity / 2;

	// 원본 슬롯의 아이템 개수를 새로 생성된 개수만큼 뺌
	ItemSlots[Index].ItemQuantity = TempItemQuantity - ItemSlots[EmptySlotIndex].ItemQuantity;

	// 효과음 재생
	FItemData* Data = USLGameDataManager::Get().GetItemDataByID(TempItemID);
	if(Data->SFXData.MoveSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Data->SFXData.MoveSound, GetOwner()->GetActorLocation());
	}
	
	// 업데이트 알림
	UpdateInventory();
}

void USLInventoryComponent::RemoveFromStack(int32 Index, int32 ItemQuantity)
{
	ItemSlots[Index].ItemQuantity -= ItemQuantity;
}

void USLInventoryComponent::PickUp(FName ItemID, int32 ItemQuantity)
{
	if(ItemQuantity <= 0) return;
	// 인벤토리 full?

	// 인벤토리에 해당 아이템 슬롯이 이미 있다면
	if(FindExistingItemSlot(ItemID) != -1)
	{
		AddToStack(FindExistingItemSlot(ItemID), ItemQuantity);
		UpdateInventory();
	}
	else
	{
		if(FindEmptySlot() == -1)
		{
			// 인벤토리 Full
			return;
		}
		else
		{
			CreateNewStack(ItemID, ItemQuantity, FindEmptySlot());
			UpdateInventory();
		}
	}
}

void USLInventoryComponent::UseItem(int32 Index)
{
	FItemSlot& Slot = ItemSlots[Index];
	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(Slot.ItemID);
	if(ItemData)
	{
		// 사용할 수 없는 아이템이면 return
		if(static_cast<EItemType>(ItemData->ItemType) == EItemType::Misc)
		{
			return;
		}
		// 장비라면 장착
		else if(static_cast<EItemType>(ItemData->ItemType) == EItemType::Armor
			|| (static_cast<EItemType>(ItemData->ItemType) == EItemType::Weapon))
		{
			// 장착 (캐릭터->EquipmentComp로 넘기기)
			OnEquipmentUsed.Broadcast(ItemData->EquipmentType, Slot.ItemID);
		}
		// 소비 아이템이라면 소비
		else
		{
			// 사용한 슬롯의 아이템 개수 조정
			int32 LeftItemQuantity = ItemSlots[Index].ItemQuantity - 1;
			if (LeftItemQuantity == 0)
			{
				ClearSlot(Index);
				// 업데이트 알림
				UpdateInventory();
			}
			else
			{
				RemoveFromStack(Index, 1);
				// 업데이트 알림
				UpdateInventory();
			}
		}
		// 효과음 재생
		if(ItemData->SFXData.UseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ItemData->SFXData.UseSound, GetOwner()->GetActorLocation());
		}
	}
}

void USLInventoryComponent::DropItem(FName ItemID, int32 ItemQuantity)
{
	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
	if (ItemData->ItemPrefab)
	{
		for (int i = 0; i < ItemQuantity; i++)
		{
			FVector SpawnLocation = GetDropLocation();
			FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetOwner();

			ASLItemBase* Item = GetWorld()->SpawnActor<ASLItemBase>(ItemData->ItemPrefab, SpawnLocation, SpawnRotation, SpawnParams);
			if(Item)
			{
				// 효과음 재생
				if(ItemData->SFXData.DropSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, ItemData->SFXData.DropSound, GetOwner()->GetActorLocation());
				}
			}
		}
	}
}

FVector USLInventoryComponent::GetDropLocation()
{
	// Owner 처리가 되고 있는지 의심됨

	AActor* Owner = Cast<AActor>(GetOwner());
	if(!Owner) return FVector::ZeroVector;

	FVector ActorLocation = Owner->GetActorLocation();
	FVector ActorForwardVector = Owner->GetActorForwardVector();

	// 캐릭터가 보고 있는 부채꼴 방향 중 랜덤한 방향 지정
	constexpr float HalfAngleDegree = 55.0f;
	FVector RandomDirection = FMath::VRandCone(ActorForwardVector, FMath::DegreesToRadians(HalfAngleDegree));

	// Drop할 범위값을 곱해줌
	constexpr float DropRange = 120.f;
	FVector RandomOffset = RandomDirection * DropRange;

	// 플레이어 위치에 RandomOffset을 더해 최종 DropLocation 계산 
	FVector DropLocation = ActorLocation + RandomOffset;

	return DropLocation;
}

uint8 USLInventoryComponent::QueryInventory(FName ItemID, int32 NeededQuantity)
{
	int32 ItemCount = 0;
	
	for(FItemSlot Slot : ItemSlots)
	{
		if(ItemID == Slot.ItemID)
		{
			ItemCount += Slot.ItemQuantity;
		}
	}

	if (ItemCount >= NeededQuantity)
		return static_cast<uint8>(true);
	else
		return static_cast<uint8>(false);
}

int32 USLInventoryComponent::GetMaxStackSize(FName ItemID)
{
	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
	if(ItemData)
	{
		return ItemData->NumericData.MaxStackSize;
	}
	return -1;
}


