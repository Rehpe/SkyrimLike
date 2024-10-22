// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/SLCharacterStatComponent.h"

#include "Character/SLCharacterPlayer.h"

// Sets default values for this component's properties
USLCharacterStatComponent::USLCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;

	CharacterStats.AttackStat = 10;
	CharacterStats.DefenceStat = 10;
	CharacterStats.MagicaStat = 10;
}


// Called when the game starts
void USLCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
}

float USLCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void USLCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	//구독자에게 HP 변경 Delegate 발행
	OnHpChanged.Broadcast(CurrentHp);
}

void USLCharacterStatComponent::CaculateFinalStat()
{
	FinalStat.AttackStat = CharacterStats.AttackStat + EquipmentModifiedStat.AttackStat;
	FinalStat.DefenceStat = CharacterStats.DefenceStat + EquipmentModifiedStat.DefenceStat;
	FinalStat.MagicaStat = CharacterStats.MagicaStat + EquipmentModifiedStat.MagicaStat;

	OnStatChanged.Broadcast();
}



