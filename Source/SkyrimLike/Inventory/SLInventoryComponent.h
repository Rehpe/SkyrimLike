// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SLInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentUsed, EEquipmentCategory, inEquipmentCategory, FName, ItemID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SKYRIMLIKE_API USLInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USLInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 이벤트
	FOnInventoryUpdated OnInventoryUpdated;

	// 장비 아이템 사용할 경우 EquipmentComponent로 넘기기
	FOnEquipmentUsed OnEquipmentUsed;

protected:
	// Inventory Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLInventoryWidget> InventoryWidget;

	// Hotbar Widget
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TObjectPtr<class USLInventoryWidget> HotbarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TArray<FItemSlot> ItemSlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 InventorySize = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HotbarSize = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float InteractRadius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	AActor* TargetActor;
	
	// 인터랙트 메세지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLInteractMessageWidget> InteractMessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLInteractMessageWidget> InteractMessageWidget;

public:
	FORCEINLINE TArray<FItemSlot>& GetItemSlots() { return ItemSlots; }
	FORCEINLINE void SetInventoryWidget(TObjectPtr<class USLInventoryWidget> inWidget) { InventoryWidget = inWidget; }
	FORCEINLINE int32 GetHotbarSize() const { return HotbarSize; }

public:
	TObjectPtr<class USLInventoryWidget> CreateInventoryWidget();

	UFUNCTION(BlueprintCallable)
	void CreateInteractMessage();

	UFUNCTION(BlueprintCallable)
	void DetectTargetActor(); // 주변 아이템 감지

	UFUNCTION(BlueprintCallable)
	void BeginDetect();

	UFUNCTION(BlueprintCallable)
	void TryInteract();
	
	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

public:
	// Slot
	UFUNCTION(BlueprintCallable)
	int32 FindExistingItemSlot(FName ItemID);

	UFUNCTION(BlueprintCallable)
	int32 FindEmptySlot();

	UFUNCTION(BlueprintCallable)
	void TransformSlot(int32 Index, int32 SourceIndex, USLInventoryComponent* SourceInventory);

	UFUNCTION(BlueprintCallable)
	void ClearSlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	void RemoveFromInventory(int32 Index, int32 RemoveQuantity);

	UFUNCTION(BlueprintCallable)
	void RemoveAllFromInventory(int32 Index);

	// Item Stack
	UFUNCTION(BlueprintCallable)
	void CreateNewStack(FName ItemID, int32 ItemQuantity, int32 Index);

	UFUNCTION(BlueprintCallable)
	void AddToStack(int32 Index, int32 ItemQuantity);

	UFUNCTION(BlueprintCallable)
	void SplitStack(int32 Index);

	UFUNCTION(BlueprintCallable)
	void RemoveFromStack(int32 Index, int32 ItemQuantity);

	// Pick UP
	UFUNCTION(BlueprintCallable)
	void PickUp(FName ItemID, int32 ItemQuantity);

	
	// Use
	UFUNCTION(BlueprintCallable)
	void UseItem(int32 Index);

	// Drop
	UFUNCTION(BlueprintCallable)
	void DropItem(FName ItemID, int32 ItemQuantity);

	UFUNCTION(BlueprintCallable)
	FVector GetDropLocation();

	// Query
	UFUNCTION(BlueprintCallable)
	uint8 QueryInventory(FName ItemID, int32 NeededQuantity);

	UFUNCTION(BlueprintCallable)
	int32 GetMaxStackSize(FName ItemID);
};
