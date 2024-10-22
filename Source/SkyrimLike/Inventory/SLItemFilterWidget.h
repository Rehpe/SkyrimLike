// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLItemFilterWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFilterClicked, bool, bIsOn, int, FilterFlag);

UCLASS()
class SKYRIMLIKE_API USLItemFilterWidget : public UUserWidget
{
	GENERATED_BODY()

	// UserWidget의 경우 FObjectInitializer& 인자를 가진 생성자만 지원함 
public:
	USLItemFilterWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	// Event
	FOnFilterClicked OnFilterClicked;

	UFUNCTION()
	void InitializeItemFilter();
	
protected:
	// Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_ItemFilter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_FilterName;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta= (AllowPrivateAccess = "true"))
	FText FilterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	uint8 bIsOn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	int32 FilterFlag;

public:
	UFUNCTION(BlueprintCallable)
	void SetFilterName();

	UFUNCTION(BlueprintCallable)
	void OnButtonItemFilterClicked();
};
