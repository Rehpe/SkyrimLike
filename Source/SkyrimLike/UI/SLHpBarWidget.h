// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLUserWidget.h"
#include "SLHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLHpBarWidget : public USLUserWidget
{
	GENERATED_BODY()
	
	// UserWidget의 경우 FObjectInitializer& 인자를 가진 생성자만 지원함 
public:
	USLHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);


protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float  MaxHp;
};
