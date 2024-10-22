// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SLGlobalLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLGlobalLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 플레이어 캐릭터 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Player")
	static class ASLCharacterPlayer* GetSLPlayerCharacter(UObject* WorldContextObject, int32 PlayerIndex = 0);

	// 플레이어 컨트롤러 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Player")
	static class ASLPlayerController* GetSLPlayerController(UObject* WorldContextObject, int32 PlayerIndex = 0);

	UFUNCTION(BlueprintCallable, Category = "Player")
	static FText GetUserName(UObject* WorldContextObject, int32 PlayerIndex = 0);
};
