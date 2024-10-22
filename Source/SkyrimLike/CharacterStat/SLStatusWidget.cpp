// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/SLStatusWidget.h"

#include "SLCharacterStatComponent.h"
#include "Character/SLCharacterPlayer.h"
#include "Components/TextBlock.h"
#include "Global/SLGlobalLibrary.h"
#include "Item/SLItemDataStructs.h"

USLStatusWidget::USLStatusWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
}

void USLStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USLStatusWidget::SetStatusText()
{
	if(StatComponent)
	{
		// 캐릭터 기본 스탯
		//FItemStat CharacterStat = StatComponent->GetCharacterStats();
		
		// 무기 보정치
		//FItemStat Equipmentstat = StatComponent->GetEquipmentModifiedStat();

		// 기타 보정치
		//

		// 최종 스탯 표시
		FItemStat FinalStat = StatComponent->GetFinalStat();
		
		Txt_AttackValue->SetText(FText::AsNumber(FinalStat.AttackStat));
		Txt_DefenceValue->SetText(FText::AsNumber(FinalStat.DefenceStat));
		Txt_MagicaValue->SetText(FText::AsNumber(FinalStat.MagicaStat));
	}
}

void USLStatusWidget::InitWidget()
{
	if(StatComponent)
	{
		StatComponent->OnStatChanged.AddDynamic(this, &USLStatusWidget::SetStatusText);
	}
	SetStatusText();
}
