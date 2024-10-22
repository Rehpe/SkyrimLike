// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SLUserWidget.h"
#include "SLHotbarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLHotbarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USLHotbarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE TObjectPtr<class UWrapBox> GetHotbarWrapBox() const { return WrapBox_Hotbar; }
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UWrapBox> WrapBox_Hotbar;

	
};
