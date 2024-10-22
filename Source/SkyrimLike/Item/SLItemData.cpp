// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SLItemData.h"
#include "SLItemBase.h"
#include "SLItemDataStructs.h"
#include "GameData/SLGameDataManager.h"

// Sets default values for this component's properties
USLItemData::USLItemData()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USLItemData::BeginPlay()
{
	Super::BeginPlay();
	
}

/*ASLItemBase* USLItemData::GetItemPrefab() const
{
	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemRowHandle.RowName);
	if(ItemData)
	{
		return Cast<ASLItemBase>(ItemData->ItemPrefab);
	}
	return nullptr;
}*/

