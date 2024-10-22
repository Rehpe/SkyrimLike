// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SLComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class SKYRIMLIKE_API USLComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USLComboActionData();

	// 몽타주 섹션 이름
	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;
	
	// 콤보 액션 개수
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	// 입력 감지용 프레임의 기준 재생속도
	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	// 입력 감지용 프레임
	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float> EffectiveFrameCount;


};
