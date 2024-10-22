// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "UI/SLUserWidget.h"
#include "Item/SLItemDataStructs.h"
#include "SLEquipmentSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotSelected, EEquipmentCategory, EquipmentCategory);

UCLASS()
class SKYRIMLIKE_API USLEquipmentSlotWidget : public USLUserWidget
{
	GENERATED_BODY()

public:
	USLEquipmentSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void InitSlotWidget(TObjectPtr<class USLEquipmentComponent> inEquipmentComponent);
	
public:
	UPROPERTY(BlueprintReadWrite)
	FOnSlotSelected OnSlotSelected;

	// Equipment Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLEquipmentComponent> EquipmentComponent;
	
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> Border_ActiveSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Img_EmptySlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Img_EquipmentSlot;
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UTexture2D> EmptySlotImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EEquipmentCategory EquipmentCategory;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetItemID(FName inItemID) { ItemID = inItemID; }
	FORCEINLINE FName GetItemID() const { return ItemID; }
	
	UFUNCTION(BlueprintCallable)
	void UpdateSlot();

	UFUNCTION(BlueprintCallable)
	void SetActive(bool bIsActive);

	UFUNCTION(BlueprintCallable)
	void SetSlot(FItemData& ItemData);
	
	UFUNCTION(BlueprintCallable)
	void SetSlotEmpty();

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

	UFUNCTION(BlueprintCallable)
	void RefreshEquipmentSlot();
};
