// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "InputActionValue.h"
#include "SLCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API ASLCharacterPlayer : public ASLCharacterBase
{
	GENERATED_BODY()
	
	public:
		ASLCharacterPlayer();
	
	protected:
		virtual void BeginPlay() override;
	
	public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Character Mesh
	UPROPERTY()
	TArray<class USkeletalMeshComponent*> MeshComps;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_Hair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_FacialHair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_Accessory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_Torso;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_Hands;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_Legs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_MeleeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh_RangedWeapon;
	
	void SetLeaderPose();

public:
	FORCEINLINE TArray<USkeletalMeshComponent*> GetMeshComps() const { return MeshComps; }
	
	// Character Control Section
	protected:
		void ChangeViewMode();
		void SetViewMode(EViewMode NewCharacterControlType);
		virtual void SetCharacterControlData(const class USLCharacterControlData* CharacterControlData) override;
	
	// Camera
	protected:
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class USpringArmComponent> SpringArm;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UCameraComponent> FollowCam;

	// PlayerInfoWidget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class USLPlayerInfoWidget> PlayerInfoWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLPlayerInfoWidget> PlayerInfoWidget;
	
	// Input
	protected:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> JumpAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> ChangeControlAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> MoveAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> AttackAction;
	
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> InteractAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> InventoryAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> ThirdPersonLookAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
		TObjectPtr<class UInputAction> FirstPersonLookAction;


		void Move(const FInputActionValue& Value);
		void ThirdPersonLook(const FInputActionValue& Value);
		void FirstPersonLook(const FInputActionValue& Value);

		EViewMode CurrentViewMode;

		void Attack();
		void Interact();
		void OpenPlayerInfo(const FInputActionValue& ActionValue);

protected:
	UFUNCTION()
	void HandleOnStatChanged();

	UFUNCTION()
	void HandleOnEquip(EEquipmentCategory inEquipmentCategory, FName ItemID);
};
