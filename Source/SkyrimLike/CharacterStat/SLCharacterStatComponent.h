// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/SLItemDataStructs.h"
#include "SLCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);	// 인자 없는 Delegate 구조체 선언
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/); // 하나의 인자를 받는 Delegate 구조체

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKYRIMLIKE_API USLCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USLCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	// Status Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USLStatusWidget> StatusWidget;

public:
	FORCEINLINE void SetStatusWidget(TObjectPtr<class USLStatusWidget> inStatusWidget) { StatusWidget = inStatusWidget; }
	
	// HP
public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)	// VisibleInstanceOnly: 인스턴스마다 다른 값 설정 가능
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

public:
	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);

protected:
	void SetHp(float NewHp);

	// Status
public:
	FOnStatChanged OnStatChanged;
	
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	FItemStat CharacterStats;
	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	FItemStat EquipmentModifiedStat;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	FItemStat FinalStat;
	
public:
	FORCEINLINE FItemStat GetCharacterStats() const { return CharacterStats; }	
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetEquipmentModifiedStat(FItemStat inEquipmentModifiedStat) { EquipmentModifiedStat = inEquipmentModifiedStat; };

	FORCEINLINE FItemStat GetEquipmentModifiedStat() const { return EquipmentModifiedStat; };

	FORCEINLINE FItemStat GetFinalStat() const { return FinalStat; };

public:
	UFUNCTION()
	void CaculateFinalStat();
	
};
