// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemBase.h"
#include "SLItemData.h"
#include "Camera/CameraLensEffectInterface.h"
#include "Character/SLCharacterPlayer.h"
#include "Components/SphereComponent.h"
#include "GameData/SLGameDataManager.h"
#include "Inventory/SLInventoryComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASLItemBase::ASLItemBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Collision->SetupAttachment(ItemMesh);

	ItemData = CreateDefaultSubobject<USLItemData>(TEXT("ItemData"));
}

// Called when the game starts or when spawned
void ASLItemBase::BeginPlay()
{
	Super::BeginPlay();

	if(ItemMesh)
	{
		ItemMesh->SetRenderCustomDepth(false);
	}
}

FText ASLItemBase::FocusTarget()
{
	if(ItemData)
	{
		FName ItemID = ItemData->GetItemID();
		FItemData* Data = USLGameDataManager::Get().GetItemDataByID(ItemID);

		if(Data)
		{
			FText ItemName = Data->TextData.Name;
			FText InteractionText = Data->TextData.InteractionText;
			
			FText Message = FText::Format(FText::FromString("{0} {1}"), ItemName, InteractionText);
			return Message;
		}
		return FText::GetEmpty();
	}
	return FText::GetEmpty();
}

void ASLItemBase::Interact(class ASLCharacterPlayer* Player)
{
	FItemData* Data = USLGameDataManager::Get().GetItemDataByID(ItemData->GetItemID());
	if(Data && Data->bIsPickable)
	{
		//Pickup
		Player->GetInventoryComponent()->PickUp(ItemData->GetItemID(),ItemData->GetItemQuantity());

		// Pickup 효과음 재생
		if(Data->SFXData.PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, Data->SFXData.PickupSound, GetActorLocation());
		}
		
		// Destroy();
		Destroy();
	}
	else
	{
		//상호작용
		if(Data->EffectClass)
		{
			//EffectClass = Cast<IItemEffectnterface>EffectClass;
			//EffectClass->Effect;
		}
		
	}
}

void ASLItemBase::OnInteract()
{
}


