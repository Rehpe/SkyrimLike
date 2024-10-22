// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemPreviewSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLItemPreviewSlotWidget : public UUserWidget
{
	GENERATED_BODY()

	// UserWidget의 경우 FObjectInitializer& 인자를 가진 생성자만 지원함 
public:
	USLItemPreviewSlotWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

public:
	FORCEINLINE void SetItemID(FName inItemID) { ItemID = inItemID; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Img_ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
};
