// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SLUserWidget.h"
#include "SLPlayerInfoWidget.generated.h"


UCLASS()
class SKYRIMLIKE_API USLPlayerInfoWidget : public USLUserWidget
{
	GENERATED_BODY()

public:
	USLPlayerInfoWidget(const FObjectInitializer& ObjectInitializer);

public:
	FORCEINLINE void SetInventoryComponent(TObjectPtr<class USLInventoryComponent> inInventoryComponent)
	{ InventoryComponent = inInventoryComponent; }

	FORCEINLINE void SetEquipmentComponent(TObjectPtr<class USLEquipmentComponent> inEquipmentComponent)
	{ EquipmentComponent = inEquipmentComponent; }

	FORCEINLINE void SetStatComponent(TObjectPtr<class USLCharacterStatComponent> inStatComponent)
	{ StatComponent = inStatComponent; }
	
protected:
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void NativeDestruct() override;

public:
	void CreateWidgets();

	void OpenWidgets();
	void CloseWidgets();
	
protected:
	// Inventory Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLInventoryComponent> InventoryComponent;

	// Equipment Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLEquipmentComponent> EquipmentComponent;

	// CharactetStat Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLCharacterStatComponent> StatComponent;
	
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USLStatusWidget> WB_StatusWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USLEquipmentWidget> WB_EquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USLInventoryWidget> WB_InventoryWidget;

public:
	uint8 bIsWidgetOn = false;

private:
	void CreateInventoryWidget();
	void CreateEquipmentWidget();
	void CreateStatusWidget();

	void DestroyInventoryWidget();
	void DestroyEquipmentWidget();
	void DestroyStatusWidget();
};
