// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/SLEquipmentComponent.h"

#include "SLCharacterWardrobe.h"
#include "SLEquipmentWidget.h"
#include "Character/SLCharacterPlayer.h"
#include "GameData/SLGameDataManager.h"
#include "Global/SLGlobalLibrary.h"
#include "Item/SLItemDataStructs.h"

// Sets default values for this component's properties
USLEquipmentComponent::USLEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ModifiedStat.AttackStat = 0.f;
	ModifiedStat.DefenceStat = 0.f;
	ModifiedStat.MagicaStat = 10.f;

	EquipmentList = {};
	EquipmentMeshSlot = {};
}

// Called when the game starts
void USLEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!EquipmentWidgetClass)
	{
		EquipmentWidgetClass = LoadClass<USLEquipmentWidget>(
			nullptr, TEXT("/Game/SkyrimLike/EquipmentSystem/UI/WB_EquipmentWidget.WB_EquipmentWidget_C"));
	}
	if (!WardrobeClass)
	{
		WardrobeClass = LoadClass<ASLCharacterWardrobe>(
			nullptr, TEXT("/Game/SkyrimLike/EquipmentSystem/BP_Wardrobe.BP_Wardrobe_C"));
	}
	UE_LOG(LogTemp, Warning, TEXT("EquipComp BeginPlay: WardrobeClass is %s, this: %p"), *GetNameSafe(WardrobeClass), this);
}

void USLEquipmentComponent::CreateWardrobe(ASLCharacterPlayer* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("EquipComp BeginPlay: WardrobeClass is %s, this: %p"), *GetNameSafe(WardrobeClass), this);
	
	// 소유자의 트랜스폼을 사용하여 스폰
	FTransform OwnerTransform = Player->GetActorTransform();
	if(Player->GetWorld())
	{
		// WardrobeClass가 유효한지 확인
		if (!WardrobeClass)
		{
			UE_LOG(LogTemp, Error, TEXT("WardrobeClass is null. Cannot spawn Wardrobe."));
			return;
		}
		
		Wardrobe = Player->GetWorld()->SpawnActor<ASLCharacterWardrobe>(WardrobeClass, OwnerTransform);
		if (Wardrobe)
		{
			Wardrobe->SetTargetActor(Player);
		}
	}
}


void USLEquipmentComponent::DestroyWardrobe()
{
	if (Wardrobe)
	{
		Wardrobe->Destroy();
		Wardrobe = nullptr;
	}
}

void USLEquipmentComponent::EquipItem(EEquipmentCategory inEquipmentCategory, FName ItemID)
{
	if (EquipmentMeshSlot.IsEmpty()) return;

	FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
	if (ItemData)
	{
		// 장착할 카테고리 enum값을 int로 변환 -> 배열에 접근(index 값이므로 -1)
		int32 SelectedCategoryIndex = static_cast<int32>(inEquipmentCategory) - 1;
		
		// EquipmentList에 장착한 아이템 추가
		EquipmentList.Add(inEquipmentCategory, ItemID);

		// ItemData에서 Mesh와 Material Interface 참조
		USkeletalMesh* Mesh = ItemData->VisualAssetData.SkeletalMesh;
		UMaterialInterface* MI = ItemData->VisualAssetData.MaterialInterface;
		
		// Mesh, Material 변경
		EquipmentMeshSlot[SelectedCategoryIndex]->SetSkeletalMeshAsset(Mesh);
		EquipmentMeshSlot[SelectedCategoryIndex]->SetMaterial(0, MI);

		ResetModifiedStats();
		CalculateModifiedStats();
		EquipmentWidget->RefreshSlots();
	}
}


void USLEquipmentComponent::CalculateModifiedStats()
{
	// EquipmentList 내의 ItemID를 가져옴
	for (const TPair<EEquipmentCategory, FName>& EquipmentPair : EquipmentList)
	{
		FName ItemID = EquipmentPair.Value;
		FItemData* ItemData = USLGameDataManager::Get().GetItemDataByID(ItemID);
		if (ItemData)
		{
			ModifiedStat.AttackStat += ItemData->ItemStat.AttackStat;
			ModifiedStat.DefenceStat += ItemData->ItemStat.DefenceStat;
			ModifiedStat.MagicaStat += ItemData->ItemStat.MagicaStat;
		}
	}

	// Stat 변경사항을 알림
	OnEquipmentStatChanged.Broadcast();
}

void USLEquipmentComponent::ResetModifiedStats()
{
	// 스탯을 초기화
	ModifiedStat.AttackStat = 0;
	ModifiedStat.DefenceStat = 0;
	ModifiedStat.MagicaStat = 0;
}
