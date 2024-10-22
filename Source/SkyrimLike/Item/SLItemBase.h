// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SLItemInteractInterface.h"
#include "SLItemBase.generated.h"

UCLASS()
class SKYRIMLIKE_API ASLItemBase : public AActor, public ISLItemInteractInterface
{
	GENERATED_BODY()
	
public:	
	ASLItemBase();

protected:
	virtual void BeginPlay() override;

protected:
	// Item Physics
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> ItemMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> Collision;

	// Item Data
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USLItemData> ItemData;
	
public:
	// Item Interact Interface
	virtual FText FocusTarget() override;
	virtual void Interact(class ASLCharacterPlayer* Player) override;
	virtual void OnInteract() override;;
};
