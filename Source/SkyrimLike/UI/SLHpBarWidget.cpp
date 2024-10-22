// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLHpBarWidget.h"
#include "Components/ProgressBar.h"	// UMG ����� Build ���Ͽ��� �����������
#include "Interface/SLCharacterWidgetInterface.h"

USLHpBarWidget::USLHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void USLHpBarWidget::NativeConstruct()
{
	// UI 관련 설정 초기화 완료 상태
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	// OwningActor(자신을 소유한 액터)에게 본인을 등록하는 절차가 필요
	// CharacterBase를 직접 참고하게 되면 의존성이 생기며, 확장에 불리함
	// 따라서 위젯을 설정하는 인터페이스를 만들고, CharacterBase가 해당 인터페이스를 상속받게끔 구조 수정
	ISLCharacterWidgetInterface* CharacterWidget = Cast<ISLCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void USLHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
