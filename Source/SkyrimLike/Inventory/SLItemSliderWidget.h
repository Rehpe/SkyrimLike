// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemSliderWidget.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseLeaved, //마우스 이벤트);

UCLASS()
class SKYRIMLIKE_API USLItemSliderWidget : public UUserWidget
{
	GENERATED_BODY()

	// UserWidget의 경우 FObjectInitializer& 인자를 가진 생성자만 지원함 
public:
	USLItemSliderWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	// Initialize
	FORCEINLINE void InitializeSlider(int32 inIndex, int32 inItemQuantity, TObjectPtr<class USLInventoryComponent> inInventoryComponent)
	{ Index = inIndex; ItemQuantity = inItemQuantity; InventoryComponent = inInventoryComponent; };

protected:
	// Inventory Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLInventoryComponent> InventoryComponent;
	
	// Slot Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity;
	
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> Border_ItemSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Cancel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Drop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class USlider> Slider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_ItemQuantity;
	
public:
	// Widget Event
	UFUNCTION(blueprintCallable)
	void OnSliderValueChanged(float Value);

	UFUNCTION(blueprintCallable)
	void OnDropButtonClicked();

	UFUNCTION(blueprintCallable)
	void OnCancelButtonClicked();
	
	// Set Slider
	UFUNCTION(blueprintCallable)
	void SetSliderValue();
	
	UFUNCTION(BlueprintCallable)
	void SetSliderText();
};
