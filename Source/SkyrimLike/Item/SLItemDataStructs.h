#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SLItemDataStructs.generated.h"


// Item Slot
USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

public:
	FItemSlot()
		: ItemQuantity(0)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity = 0;
	
	// == 연산자 오버로딩
	bool operator==(const FItemSlot& Other) const
	{
		// ItemID와 Quantity가 모두 같을 때만 true 반환
		return this->ItemID == Other.ItemID && this->ItemQuantity == Other.ItemQuantity;
	}
};

UENUM(BlueprintType, meta = (Bitmask, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EItemType : uint8
{
	None   = 0 UMETA(Hidden),  // 기본값 (0)
	Weapon = 1 << 0 UMETA(DisplayName = "Weapon"), // 1
	Armor = 1 << 1 UMETA(DisplayName = "Armor"), // 2
	Consumable = 1 << 2 UMETA(DisplayName = "Consumable"), // 4
	Misc = 1 << 3 UMETA(DisplayName = "Misc"), // 8
};
ENUM_CLASS_FLAGS(EItemType);

UENUM(BlueprintType)
enum class EEquipmentCategory : uint8
{
	None   = 0 UMETA(DisplayName ="None"),  // 기본값 (0)
	Hair UMETA(DisplayName ="Hair"),
	FacialHair UMETA(DisplayName ="FacialHair"),
	Accessory UMETA(DisplayName ="Accessory"),
	Torso UMETA(DisplayName ="Torso"),
	Hands UMETA(DisplayName ="Hands"),
	Legs UMETA(DisplayName ="Legs"),
	MeleeWeapon UMETA(DisplayName ="MeleeWeapon"),
	RangedWeapon UMETA(DisplayName ="RangedWeapon"),
};
ENUM_CLASS_FLAGS(EEquipmentCategory);

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	None   = 0 UMETA(DisplayName ="None"),  // 기본값 (0)
	Hp UMETA(DisplayName ="Hp"),
	Mp UMETA(DisplayName ="Mp"),
};
ENUM_CLASS_FLAGS(EAttributeType);

USTRUCT(BlueprintType)
struct FItemStat
{
	GENERATED_BODY()

public:
	FItemStat()
		: AttackStat(0.f), DefenceStat(0.f), MagicaStat(0.f), RestorationAmount(0.f)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	// 공격력
	float AttackStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	// 방어력
	float DefenceStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	// 마력
	float MagicaStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	// 회복량
	float RestorationAmount;
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_BODY()

public:
        FItemTextData()
            : Name(FText::GetEmpty()), Description(FText::GetEmpty()), InteractionText(FText::GetEmpty()), UsageText(FText::GetEmpty())
        {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText UsageText;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

public:
	FItemNumericData()
		: Weight(0.0f), bIsStackable(false), MaxStackSize(1), Price(0.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	// 가격
	float Price;
};

USTRUCT(BlueprintType)
struct FItemSFXData
{
	GENERATED_BODY()

public:
	FItemSFXData()
		: PickupSound(nullptr), DropSound(nullptr), MoveSound(nullptr), UseSound(nullptr)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* DropSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* MoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* UseSound;
};

USTRUCT(BlueprintType)
struct FItemVisualAssetData
{
	GENERATED_BODY()

public:
	FItemVisualAssetData()
		: Icon(nullptr), SkeletalMesh(nullptr), MaterialInterface(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MaterialInterface;
};

USTRUCT(BlueprintType)
struct FItemVFXData
{
	GENERATED_BODY()

public:
	FItemVFXData()
		: UseEffect(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* UseEffect;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData()
		: ID(NAME_None), bIsPickable(true), ItemType(static_cast<int32>(EItemType::None)), 
		  EquipmentType(EEquipmentCategory::None), AttributeType(EAttributeType::None),
		  EffectClass(nullptr), ItemPrefab(nullptr)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	uint8 bIsPickable = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data", meta = (Bitmask, BitmaskEnum = "/Script/SkyrimLike.EItemType"))
	int32 ItemType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EEquipmentCategory EquipmentType = EEquipmentCategory::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EAttributeType AttributeType = EAttributeType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Item Data")
	FItemStat ItemStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemVisualAssetData VisualAssetData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemSFXData SFXData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemVFXData VFXData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSubclassOf<AActor> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSubclassOf<AActor> ItemPrefab;
};