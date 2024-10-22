// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterNonPlayer.h"

ASLCharacterNonPlayer::ASLCharacterNonPlayer()
{

}

void ASLCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	// 람다 함수를 사용해 스스로를 Destroy 하는 함수 실행
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}
