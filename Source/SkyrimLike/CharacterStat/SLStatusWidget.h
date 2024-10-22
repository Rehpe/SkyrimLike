// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SLUserWidget.h"
#include "SLStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLStatusWidget : public USLUserWidget
{
	GENERATED_BODY()

	// UserWidget의 경우 FObjectInitializer& 인자를 가진 생성자만 지원함 
public:
	USLStatusWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

protected:
	// CharacterStat Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class USLCharacterStatComponent> StatComponent;

public:
	FORCEINLINE void SetStatComponent(TObjectPtr<class USLCharacterStatComponent> InStatComponent) { StatComponent = InStatComponent; }
	
protected:
	// Stat Value Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_AttackValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_DefenceValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_MagicaValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (BindWidget))
	TObjectPtr<class UTextBlock> Txt_MoveSpeedValue;

public:
	UFUNCTION(BlueprintCallable)
	void SetStatusText();

	UFUNCTION(BlueprintCallable)
	void InitWidget();

};
