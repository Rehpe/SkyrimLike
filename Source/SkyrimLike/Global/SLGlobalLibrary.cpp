// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/SLGlobalLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Character/SLCharacterPlayer.h"
#include "Player/SLPlayerController.h"

class ASLCharacterPlayer* USLGlobalLibrary::GetSLPlayerCharacter(UObject* WorldContextObject, int32 PlayerIndex)
{
	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex))
	{
		return Cast<ASLCharacterPlayer>(PlayerCharacter);
	}
	return nullptr;
}

class ASLPlayerController* USLGlobalLibrary::GetSLPlayerController(UObject* WorldContextObject, int32 PlayerIndex)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex))
	{
		return Cast<ASLPlayerController>(PlayerController);
	}
	return nullptr;
}

FText USLGlobalLibrary::GetUserName(UObject* WorldContextObject, int32 PlayerIndex)
{
	if(ACharacter* PlayerCharacter = GetSLPlayerCharacter(WorldContextObject, PlayerIndex))
	{
		if(ASLCharacterPlayer* SLCharacterPlayer = Cast<ASLCharacterPlayer>(PlayerCharacter))
		{
			return FText::FromString(SLCharacterPlayer->GetName());
		}
	}
	return FText::FromString(TEXT(""));
}
