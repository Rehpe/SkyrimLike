// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLInteractMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLInteractMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	// UserWidget의 경우 FObjectInitializer& 인자를 가진 생성자만 지원함 
public:
	USLInteractMessageWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Txt_InteractMessage;
	//TObjectPtr<class UTextBlock> Txt_InteractMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractMessage;

public:
	UFUNCTION(BlueprintCallable)
	void SetInteractMessage(const FText& Message);
};
