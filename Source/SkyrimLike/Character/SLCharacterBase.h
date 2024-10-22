// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SLAnimationAttackInterface.h"
#include "Interface/SLCharacterWidgetInterface.h"
//#include "UI/SLPlayerInfoWidget.h"
#include "SLCharacterBase.generated.h"

UENUM()
enum class EViewMode : uint8
{
	ThirdPersonView,
	FirstPersonView,
};

UCLASS()
class SKYRIMLIKE_API ASLCharacterBase : public ACharacter, public ISLAnimationAttackInterface, public ISLCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASLCharacterBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	virtual void SetCharacterControlData(const class USLCharacterControlData* CharacterControlData);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
		TMap<EViewMode, class USLCharacterControlData*>	ViewModeManager;

	//Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class USLComboActionData> ComboActionData;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();	// 타이머 발동
	void ComboCheck();	// 입력 여부 감지

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;	// 원하는 시간에 함수 호출할 수 있는 타이머
	bool HasNextComboCommand = false;

	// Attack Hit
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Dead 
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

protected:
	// StatComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLCharacterStatComponent> Stat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLEquipmentComponent> EquipmentComponent;

public:
	FORCEINLINE TObjectPtr<class USLCharacterStatComponent> GetStatComponent() const { return Stat; }
	FORCEINLINE TObjectPtr<class USLInventoryComponent> GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE TObjectPtr<class USLEquipmentComponent> GetEquipmentComponent() const { return EquipmentComponent; }
	
protected:
	// UI Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class USLUserWidget* InUserWidget) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// Player Inventory Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> DisplayWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDWidget;
};
