// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SLUserWidget.h"
#include "SLEquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLEquipmentWidget : public USLUserWidget
{
	GENERATED_BODY()

	// UserWidget의 경우 FObjectInitializer& 인자를 가진 생성자만 지원함 
public:
	USLEquipmentWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION()
	void InitWidget();
	
	FORCEINLINE void SetEquipmentComponent(TObjectPtr<class USLEquipmentComponent> InEquipmentComponent) { EquipmentComponent = InEquipmentComponent;};

protected:
	// Equipment Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLEquipmentComponent> EquipmentComponent;
	
	// Equipment Slots
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TArray<TObjectPtr<class USLEquipmentSlotWidget>> EquipmentSlots;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_FacialHair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_Accessory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_Torso;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_Hands;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_Legs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_MeleeWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class USLEquipmentSlotWidget> Slot_RangedWeapon;

	// Text Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_LevelValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_PlayerName;

protected:
	// Active Slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLEquipmentSlotWidget> ActiveSlot;

public:
	FORCEINLINE TObjectPtr<class USLEquipmentSlotWidget> GetActiveSlot() const { return ActiveSlot; };
	void RefreshSlots();
	
protected:
	// Available Equipment Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLAvailableEquipmentInventoryWidget> AvailableEquipmentInventoryClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLAvailableEquipmentInventoryWidget> AvailableEquipmentInventory;

private:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void SetUserNameText();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void InitSlots();
	
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void ToggleSlotActivation(EEquipmentCategory inEquipmentType);
	
	void SetEquipment(EEquipmentCategory inEquipmentCategory,
		TObjectPtr<class USLEquipmentSlotWidget> EquippedSlotWidget,
		FName ItemID);


};
