// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/SLGameDataManager.h"

USLGameDataManager::USLGameDataManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>  ItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/SkyrimLike/InventorySystem/ItemData/ItemDataTable.ItemDataTable'"));
	if (ItemDataTableRef.Succeeded())
	{
		ItemDataTable = ItemDataTableRef.Object;
		check(ItemDataTable->GetRowMap().Num() > 0);
	}
}

USLGameDataManager& USLGameDataManager::Get()
{
	USLGameDataManager*  Singleton = CastChecked<USLGameDataManager>(GEngine->GameSingleton);
	if(Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogTemp, Error, TEXT("Singleton 반환 실패"));
	return *NewObject<USLGameDataManager>();	// 반환을 위한 코드로, 실제 사용하지 않음
}

FItemData* USLGameDataManager::GetItemDataByID(FName ItemID) const
{
	if (ItemDataTable)
	{
		static const FString ContextString(TEXT("ItemDataLookup"));  // 로그에 출력될 문자열 (필수)
        
		// 데이터 테이블에서 ItemID에 해당하는 행을 찾음
		FItemData* ItemData = ItemDataTable->FindRow<FItemData>(ItemID, ContextString);
		if (ItemData)
		{
			return ItemData;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Item ID %s not found in Item Data Table"), *ItemID.ToString());
		}
	}
	return nullptr;
}

