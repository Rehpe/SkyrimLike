// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SLEquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentStatChanged);

struct FItemStat;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKYRIMLIKE_API USLEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USLEquipmentComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	FOnEquipmentStatChanged OnEquipmentStatChanged;
	
public:
	FORCEINLINE void SetEquipmentWidget(TObjectPtr<class USLEquipmentWidget> inWidget) { EquipmentWidget = inWidget; }
	FORCEINLINE TMap<EEquipmentCategory, FName> GetEquipmentList() { return EquipmentList; }
	FORCEINLINE void SetEquipmentMeshSlot(TArray<class USkeletalMeshComponent*>& MeshSlot) { EquipmentMeshSlot = MeshSlot; }
	
protected:
	// Equipment List
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EEquipmentCategory, FName> EquipmentList;
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly)
	TArray<class USkeletalMeshComponent*> EquipmentMeshSlot;
	
	// Equipment Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USLEquipmentWidget> EquipmentWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USLEquipmentWidget> EquipmentWidget;

public:
	// Wardrobe Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ASLCharacterWardrobe> WardrobeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ASLCharacterWardrobe> Wardrobe;

	// Modified Stat
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FItemStat ModifiedStat;

public:
	FORCEINLINE FItemStat GetModifiedStat() const { return ModifiedStat; }
	
public:
	UFUNCTION(BlueprintCallable)
	void CreateWardrobe(ASLCharacterPlayer* Player);

	//UFUNCTION(BlueprintCallable)
	//void CreateWardrobe();
	
	UFUNCTION(BlueprintCallable)
	void DestroyWardrobe();

public:
	UFUNCTION(BlueprintCallable)
	void EquipItem(EEquipmentCategory inEquipmentCategory, FName ItemID);
	
	UFUNCTION(BlueprintCallable)
	void CalculateModifiedStats();

	UFUNCTION()
	void ResetModifiedStats();
};
