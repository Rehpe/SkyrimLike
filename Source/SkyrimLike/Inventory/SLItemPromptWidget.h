// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemPromptWidget.generated.h"

UCLASS()
class SKYRIMLIKE_API USLItemPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USLItemPromptWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	void SetWidgetPositionToMouse();

public:
	// Initialize
	//UFUNCTION(BlueprintCallable)
	FORCEINLINE void InitializePrompt(int32 inIndex, TObjectPtr<class USLInventoryComponent> inInventoryComponent)
	{ Index = inIndex; InventoryComponent = inInventoryComponent; };

protected:
	// Inventory Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLInventoryComponent> InventoryComponent;

	// Index
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;
	
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UBorder> Border_ItemPrompt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Use;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_Use;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Drop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Split;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Inspect;

	// ItemSlider
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLItemSliderWidget> ItemSliderWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLItemSliderWidget> ItemSlider;

	// Event Binding
	UFUNCTION(BlueprintCallable)
	void Button_UseClicked();

	UFUNCTION(BlueprintCallable)
	void Button_DropClicked();

	UFUNCTION(BlueprintCallable)
	void Button_SplitClicked();

	UFUNCTION(BlueprintCallable)
	void Button_InspectClicked();
	
	
	
};
